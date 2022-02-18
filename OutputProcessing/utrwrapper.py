#!/usr/bin/env python3

import argparse
import os
import sys
import subprocess
import configparser
import glob
import datetime

startTime=datetime.datetime.now()
programName=os.path.basename(sys.argv[0])

# Parse command line arguments in an nice way with automatic usage message etc
argparser = argparse.ArgumentParser(description="""
Run utr simulations controlled by extended macro files combining a utr macro
file with a configuration header

An extended macro file is a regular utr/GEANT4 macro file with a configuration
header embedded as comment lines in the macro (lines proceeded by a '#').
"""+ programName +""" reads this header and based on it prepares the simulation
(e.g. creating directories, aborting on already existing output files, configuring
utr with required build options and making it), conducts the simulation defined
by the macro file (with required niceness, threads and output directory), and
optionally does subsequent output processing, while also optionally documenting
all steps in a logfile.
""",
epilog="""
Below is an example configuration header, illustrating its structure and
listing all available options with their effect and default values
#!/usr/bin/env """+ programName +"""
#[generalConfig]                      # Required section with general configuration
#                                     # (all options are optional however)
#logging=True                         # Whether to create a logfile in the
#                                     # output directory (Default: True)
#niceness=18                          # Niceness to run utr with (Default: 18)
#threads=20                           # Number of threads to run utr and make
#                                     # with (Default: System's CPU count)
#utrPath=/path/to/utr                 # Path of utr's code (Default: Parent
#                                     # directory of """+ programName +"""'s directory)
#checkForExistingOutput=True          # Whether to check for already existing
#                                     # output files in the output directory
#                                     # and potentially abort (Default: True,
#                                     # ignored when -s option is supplied to
#                                     # call of """+ programName +""")
#processOutput=True                   # Whether to process the output with
#                                     # getHistogram and histogramToTxt after
#                                     # the simulation ended (Default: True)
#getHistogramExe=My-getHistogram      # The getHistogram executeable/version
#                                     # to use
#outputDir=/path/to/utr/output        # The output directory
#separateRawOutputAndHistograms=True  # Whether to separate utr's output from
#                                     # the processed output by two subfolders
#                                     # (Raw_Events and Event_Histograms) in
#                                     # the output directory (Default: True)
#cdToUtrPath=True                     # Whether to change the working directory
#                                     # to utr's code directory and using all
#                                     # other supplied paths relative to it
#                                     # (Default: True)
#ensureTerminalMultiplexer=False      # Whether to warn and abort if """+ programName +""" 
#                                     # is not run inside a tmux or GNU Screen 
#                                     # session (Default: False)
#cmakeArgs=--debug-output             # Pure string of additional arguments to
#                                     # cmake on utr, see also [utrBuildOptions]
#                                     # section (Default: None)
#getHistogramArgs=-s                  # Pure string of additional arguments to
#                                     # getHistogram, see also [getHistogramArgs]
#                                     # section (Default: None)
#histogramToTxtArgs=--skipempty       # Pure string of additional arguments to
#                                     # histogramToTxt, see also [histogramToTxtArgs]
#                                     # section (Default: None)

## Use multiple '#' for comments in the header

#[environmentVariables]               # Semi-optional section of environment
#                                     # variables to be supplied to the call
#                                     # to utr (can be accessed in the
#                                     # utr/GEANT4 macro by the /control/getEnv
#                                     # command)
#filenamePrefix=Efficiency_           # Filename prefix of utr's output files,
#                                     # required to identify output files when
#                                     # processOutput or checkForExistingOutput
#                                     # is True
#filenameSuffix=                      # Filename suffix of utr's output files,
#                                     # required to identify output files when
#                                     # processOutput or checkForExistingOutput
#                                     # is True (Here: Suffix is an empty string
#                                     # (no suffix), in such cases watch out to
#                                     # not use the {filenameSuffix} variable in
#                                     # the utr/GEANT4 macro, as GEANT4 cannot
#                                     # handle aliases with an empty value!)
#myVar=Value                          # Example

#[utrBuildOptions]                    # Optional section with cmake build options
#                                     # for utr of the form BUILDOPTION=VALUE
#CAMPAIGN=Campaign_2014_2015          # Example
#DETECTOR_CONSTRUCTION=150Sm          # Example
#USE_TARGETS=ON                       # Example
#GENERATOR_ANGCORR=OFF                # Example
#GENERATOR_ANGDIST=ON                 # Example
#ZERODEGREE_OFFSET=30                 # Example

#[getHistogramArgs]                   # Optional section with additional longform
#                                     # options (--LONGOPTION=VALUE) to getHistogram
#                                     # of the form LONGOPTION=ARGUMENT
#maxenergy=10                         # Example
#binning=2                            # Example

#[histogramToTxtArgs]                 # Optional section with additional longform
#                                     # options (--LONGOPTION=VALUE) to histogramToTxt
#                                     # of the form LONGOPTION=ARGUMENT
#begin=1                              # Example
#end=4                                # Example
#countsonly=                          # Example

#START_OF_MACRO                       # Required magic string signaling the end of
#                                     # the configuration header
#Here the actual utr/GEANT4 macro would start
""", formatter_class=argparse.RawTextHelpFormatter)
argparser.add_argument('macFile', metavar='EXTENDEDMACROFILE', help='the extended macro file to process')
argparser.add_argument('-s', '--skipSimulation', action='store_true', help="""\
skip simulation with utr (e.g. to only process output of a
previous simulation)
""")
args=argparser.parse_args()

# The given extended macro file must exist
macFile=os.path.realpath(args.macFile)
if not os.path.isfile(macFile) :
    exit("ERROR: Supplied extended macro file path'" + macFile + "' is no valid file! Aborting...")

# Extract the configuration from the extended macro file by reading line by line
cfgParts=[]
with open(macFile) as openMacFile:
    for line in openMacFile:
        # First strip leading whitespace
        line=line.lstrip()
        # If magic string signaling end of configuration is encountered stop reading
        if line.startswith("#START_OF_MACRO") :
            break
        # Ignore the shebang line
        if line.startswith("#!") :
            continue
        # Accept all other comment lines, uncomment them (remove the first leading hash) and add them to the configuration
        elif line.startswith("#") :
            cfgParts.append(line[1:])
        # Empty lines (whitespaces only) are also ok
        elif line.strip()=="" :
            cfgParts.append(line)
        # All other lines in the configuration part are forbidden
        else :
            exit("ERROR: Malformed extended macro file: Encountered non-empty line '\n"+line+"' without a leading '#' in the configuration header of the extended macro file (before '#START_OF_MACRO' line)! Aborting...")

# Parse the extracted configuration
config=configparser.ConfigParser(inline_comment_prefixes='#')
config.optionxform=lambda option: option # Keep keys' case as is, overwrites the default of converting keys to lowercase
config.read_string("".join(cfgParts))
if not config.has_section("generalConfig") :
    exit("ERROR: Extended macro file is missing required section '[generalConfig]' in its configuration header")

# If required by config, script will abort if not run in a TMUX or Screen session
if config["generalConfig"].getboolean("ensureTerminalMultiplexer", False) and 'TMUX' not in os.environ and 'STY' not in os.environ :
    exit("ERROR: " + os.path.basename(sys.argv[0]) + " is not run within a tmux or screen session as required by the configuration header! Aborting...")

# Get all options from the configuration
logging=config["generalConfig"].getboolean("logging", True)
niceness=config["generalConfig"].get("niceness", 18)
threads=config["generalConfig"].get("threads", os.cpu_count())
utrPath=config["generalConfig"].get("utrPath",
    os.path.realpath(
        os.path.join(
            os.path.dirname(__file__),
            "..")))
checkForExistingOutput=config["generalConfig"].getboolean("checkForExistingOutput", True)
processOutput=config["generalConfig"].getboolean("processOutput", True)
getHistogramExe=config["generalConfig"].get("getHistogramExe", "getHistogram")
outputDir=config["generalConfig"].get("outputDir", "output")
outputDirRaw=outputDir
outputDirHists=outputDir
if (config["generalConfig"].getboolean("separateRawOutputAndHistograms", True)) :
    outputDirRaw=os.path.join(outputDirRaw, "Raw_Events")
    outputDirHists=os.path.join(outputDirHists, "Event_Histograms")

# For processing of output the composition of the filename must be set in the configuration
if processOutput or checkForExistingOutput or logging :
    if "environmentVariables" not in config or "filenamePrefix" not in config["environmentVariables"] or "filenameSuffix" not in config["environmentVariables"] :
        exit("ERROR: To process the output, write a log, or check for already existing output the keys 'filenamePrefix' and 'environmentVariables' must be set in the '[environmentVariables]' section of the configuration header in the extended macro file! Aborting...")
    filenamePrefix=config["environmentVariables"]["filenamePrefix"]
    filenameSuffix=config["environmentVariables"]["filenameSuffix"]

# Gather environment variables from the config which shall be supplied to the call to utr
# These can then be accessed in the utr/GEANT4 macro by the /control/getEnv command
environmentVariables=dict(os.environ)
if "environmentVariables" in config :
    environmentVariables.update(config["environmentVariables"])

# Check if utr can be found
if not os.path.isfile(os.path.join(utrPath,"src","utr.cc")) :
    exit("ERROR: Could not find utr in utrPath '" + utrPath + "'! Aborting...")

# If requested change the working directory to utr's directory, the given output directory will then be used relative to this path
if config["generalConfig"].getboolean("cdToUtrPath", True) :
    os.chdir(utrPath)

# Gather cmake arguments and build options from the configuration
cmakeArgs=[]
if "cmakeArgs" in config["generalConfig"] :
    cmakeArgs+=config["generalConfig"]["cmakeArgs"].split()
if "utrBuildOptions" in config :
    for opt in config["utrBuildOptions"] :
        cmakeArgs.append("-D" + opt + "=" + config["utrBuildOptions"][opt])

# Create output directories including parent folders
os.makedirs(outputDirRaw, exist_ok=True)
os.makedirs(outputDirHists, exist_ok=True)

# Check if output directory already contains files matching the supplied filename structure
if checkForExistingOutput and not args.skipSimulation and glob.glob(
    glob.escape(os.path.join(outputDirRaw, filenamePrefix))
    + "*"
    + glob.escape(filenameSuffix)
    + "*"
) :
    exit("ERROR: Output directory seems to contain files matching the supplied filename structure! Aborting...")


# Set logfile path if logging is requested
if logging :
    logfilePath=os.path.join(outputDir, filenamePrefix + "X" + filenameSuffix + "_" + startTime.strftime("%Y-%m-%d_%H-%M-%S") + ".log")

# Define combined print and log function
def loggingPrint(*args, tag=programName + ">", **kwargs):
    print(tag, *args, **kwargs)
    if logging :
        with open(logfilePath, "a") as f:
            kwargs["file"]=f
            print(tag, *args, **kwargs)

# Define function to print and log errors and quit
def error(*args, **kwargs) :
    # '*(args + tuple(["Aborting..."]))' instead of '*args, append' for python < 3.5 compatibility
    loggingPrint(*(args + tuple(["Aborting..."])), tag="ERROR:", file=sys.stderr, **kwargs)
    exit(1)

# Print and log startup info
loggingPrint("Started processing extended macro file '" + macFile + "' at", startTime.strftime("%Y-%m-%d %H-%M-%S"))
if args.skipSimulation :
    loggingPrint("Command-line argument '--skipSimulation' was supplied. Simulation will be skipped!")

# Dump extended and also referred macro files to the logfile
if logging :
    fileType="extended macro"
    fileQueue=[macFile]
    for mac in fileQueue :
        if not os.path.isfile(mac) :
            error("Referred macro file '" + mac + "' not found!")
        with open(logfilePath, "a") as logfile:
            print(programName + ">", "Contents of", fileType, "file '" + mac + "':", file=logfile)
            with open(mac) as openMac:
                # Inspect every line in the macro, if it is a known GEANT4 command executing another macro, queue that macro for dumping and inspecting, if it was not already queued
                for line in openMac :
                    # Dump the macro file to the logfile line by line with a tag prepended
                    logfile.write("Dump of " + os.path.basename(mac) + "> " + line)
                    # Inspect the line
                    splitLine=line.split()
                    if len(splitLine)==0 :
                        continue
                    for (cmd, pos) in [("execute", 1), ("loop", 1), ("foreach", 1), ("if", 4), ("strif", 4), ("ifBatch", 1), ("ifInteractive", 1)] :
                        if splitLine[0] == "/control/" + cmd and splitLine[pos] not in fileQueue:
                            fileQueue.append(splitLine[pos])
                if not line.endswith("\n") :
                    logfile.write("\n")
            print(programName + ">", "End of", fileType, "file '" + mac + "'", file=logfile)
        fileType="referred macro"


# Define function to run processes, respecting logging and checking exit status
def runProcess(prog, procArgs, announce=True, **kwargs) :
    if announce :
        loggingPrint("Running", prog)
    if not logging :
        if (subprocess.run(procArgs, **kwargs).returncode != 0) :
            error(prog, "failed!")
    else :
        with subprocess.Popen(procArgs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True, **kwargs) as proc :
            for line in proc.stdout:
                loggingPrint(line, tag=prog+">", end="")
            if (proc.wait() != 0) :
                error(prog, "failed!")


# Compile the programs
if not args.skipSimulation :
    runProcess("cmake on utr", ["cmake","."] + cmakeArgs, cwd=utrPath)
    runProcess("buildsystem on utr", ["cmake", "--build", ".", "-j" , threads], cwd=utrPath)
if processOutput :
    outputProcessingPath=os.path.join(utrPath, "OutputProcessing")
    if not os.path.isdir(outputProcessingPath) :
        error("Could not find OutputProcessing directory!")
    getHistogramArgs=[] # Gather getHistogramArgs options from the configuration
    if "getHistogramArgs" in config["generalConfig"] :
        getHistogramArgs+=config["generalConfig"]["getHistogramArgs"].split()
    if "getHistogramArgs" in config :
        for opt in config["getHistogramArgs"] :
            if config["getHistogramArgs"][opt] == "" :
                getHistogramArgs.append("--" + opt)
            else :
                getHistogramArgs.append("--" + opt + "=" + config["getHistogramArgs"][opt])
    histogramToTxtArgs=[] # Gather histogramToTxtArgs options from the configuration
    if "histogramToTxtArgs" in config["generalConfig"] :
        histogramToTxtArgs+=config["generalConfig"]["histogramToTxtArgs"].split()
    if "histogramToTxtArgs" in config :
        for opt in config["histogramToTxtArgs"] :
            if config["histogramToTxtArgs"][opt] == "" :
                histogramToTxtArgs.append("--" + opt)
            else :
                histogramToTxtArgs.append("--" + opt + "=" + config["histogramToTxtArgs"][opt])
    runProcess("cmake on OutputProcessing scripts", ["cmake", "."], cwd=outputProcessingPath)
    runProcess("buildsystem on OutputProcessing scripts", ["cmake", "--build", ".", "-j", str(threads)], cwd=outputProcessingPath)

# Run utr (if not skipped)
if not args.skipSimulation :
    runProcess("utr", [
        "nice", "-n" + str(niceness),
        os.path.join(utrPath, "utr"),
        "--nthreads=" + str(threads),
        "--outputdir=" + outputDirRaw + "",
        "--macrofile=" + macFile + ""
        ],
        env=environmentVariables
    )

# Process the output if requested
if processOutput :
    loggingPrint("Processing output...")
    # Get a list of all simulation output basenames (without _t{THREADID}.root) by the known composition of the output filenames and the always present _t0 file
    outputFiles=glob.glob(
        glob.escape(os.path.join(outputDirRaw, filenamePrefix))
        + "*"
        + glob.escape(filenameSuffix + "_t0.root")
    )
    loggingPrint("Found", len(outputFiles), "output files to process")
    for rootFilename in outputFiles :
        loggingPrint("Processing output file '" + rootFilename.rpartition("_t")[0] + "'...")
        # Strip _t0.root and directory name from string
        rootFilename=os.path.basename(rootFilename).rpartition("_t")[0]
        runProcess(getHistogramExe, [
            os.path.join(outputProcessingPath, getHistogramExe),
            "--pattern1=" + rootFilename + "_t",
            "--inputdir=" + outputDirRaw,
            "--outputdir=" + outputDirHists
            ] + getHistogramArgs, announce=False)
        runProcess("histogramToTxt", [
            os.path.join(outputProcessingPath, "histogramToTxt"),
            os.path.join(outputDirHists, rootFilename) + "_hist.root"
            ] + histogramToTxtArgs, announce=False)

endTime=datetime.datetime.now()
elapsedSeconds=int((endTime-startTime).total_seconds())
loggingPrint("Finished processing extended macro file '" + macFile + "' at", datetime.datetime.now().strftime("%Y-%m-%d %H-%M-%S"), f"(took {elapsedSeconds//(60*60)}h {(elapsedSeconds//60)%60:02}m {elapsedSeconds%60:02}s)")

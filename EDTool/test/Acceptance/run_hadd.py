import argparse
import os, sys

# -- usage:
# -- e.g python run_hadd.py --inputPath=/user/kplee/SE/DYAccPlot_v1

def consistencyCheck(dic_sampleType_path, sampleType):
  for key in dic_sampleType_path.keys():
    if sampleType == key:
      print("sampleType = %s already exists!")
      print("--> existing path = %s" % dic_sampleType_path[sampleType] )
      print("--> new path = %s" % path )
      sys.exit()

if __name__ == '__main__':
  parser = argparse.ArgumentParser()

  parser.add_argument('--inputPath',
                      type = str,
                      help = 'Path to the directory where the CRAB outputs (i.e. plots) are stored')

  args = parser.parse_args()

  print("--- input argument ---")
  print("inputPath = %s" % args.inputPath)
  print("----------------------\n")

  dic_sampleType_path = {}

  for (path, list_dirName, list_fileName) in os.walk(args.inputPath):
    isLastDir = False
    for fileName in list_fileName:
      if ".root" in fileName:
        isLastDir = True
        break

    if not isLastDir: continue

    sampleType = ""
    list_split = path.split("/")
    for split in list_split:
      if "crab_" in split:
        sampleType = split.split("crab_")[-1]

    # -- check whether there are more than 2 paths for a given sampleType (which is not expected in a usual case)
    consistencyCheck(dic_sampleType_path, sampleType)

    dic_sampleType_path[sampleType] = path
  # -- end of loop over os.walk()

  # for key in dic_sampleType_path.keys():
  #   print("%s --> %s" % (key, dic_sampleType_path[key]) )

  if "output" not in os.listdir("."):
    os.mkdir("output")
    
  for key in dic_sampleType_path.keys():
    sampleType = key
    rootFilePath = dic_sampleType_path[key]
    
    mergedFileName = "output/%s.root" % (sampleType)
    cmd_hadd = "hadd %s %s/*.root" % (mergedFileName, rootFilePath)
    print(cmd_hadd)
    # print("\n")

    os.system(cmd_hadd)










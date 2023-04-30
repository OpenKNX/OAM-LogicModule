Import("env")
import gzip
import re

def post_program_action(source, target, env):
    print("Building ", source[0].get_path() + ".gz")
    with open(source[0].get_abspath(), 'rb') as orig_file:
        with gzip.open(source[0].get_abspath() + ".gz", 'wb') as zipped_file:
            zipped_file.writelines(orig_file)

def post_program_uf2(source, target, env):
    print("Building ", source[0].get_path()[0:-4] + "_new.uf2")
    with open(env["PROJECT_DIR"] + "/../OGM-Common/include/knxprod.h", 'r') as knxprod:
        content = knxprod.read(-1)

    m = re.search("#define MAIN_OpenKnxId 0x([0-9A-Fa-f]{2})", content)
    openKnxId = m.group(1)
    m = re.search("#define MAIN_ApplicationNumber ([0-9A-Fa-f]{2})", content)
    appNumber = m.group(1)
    m = re.search("#define MAIN_ApplicationVersion ([0-9A-Fa-f]{2})", content)
    appVersion = m.group(1)
    
    with open(env["PROJECT_SRC_DIR"] + "/main.cpp", 'r') as knxprod:
        content = knxprod.read(-1)

    m = re.search("firmwareRevision = ([0-9]+);", content)
    appRevision = m.group(1)    
    print("Id: " + openKnxId + ", Number: " + appNumber + ", Version: " + appVersion + ", Revision: " + appRevision)
    with open(source[0].get_path()[0:-4] + ".uf2", "rb") as orig_file:
        barray=bytearray(orig_file.read())
        barray[9] = barray[9] | 0x80
        barray[288] = 8 #Tag Size
        barray[289] = ord('K') #Type
        barray[290] = ord('N') #Type
        barray[291] = ord('X') #Type
        barray[292] = int(openKnxId, 16) #Data
        barray[293] = int(appNumber) #Data
        barray[294] = int(appVersion) #Data
        barray[295] = int(appRevision) #Data
        with open(source[0].get_path()[0:-4] + "_new.uf2","wb") as file: 
            file.write(barray)

env.AddPostAction("buildprog", post_program_uf2)
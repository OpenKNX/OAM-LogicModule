Import("env")
import gzip
import subprocess, sys

def post_program_action(source, target, env):
    print("Building ", source[0].get_path() + ".gz")
    with open(source[0].get_abspath(), 'rb') as orig_file:
        with gzip.open(source[0].get_abspath() + ".gz", 'wb') as zipped_file:
            zipped_file.writelines(orig_file)

def post_program_uf2(source, target, env):
    print("Building ", source[0].get_path()[0:-4] + "_new.uf2")
    p = subprocess.Popen(["powershell.exe", 
                "(scripts/Get-Version-Information.ps1).UF2Version"], 
                stdout=subprocess.PIPE)
    (uf2Version, err) = p.communicate()
    uf2Version = uf2Version.decode('ascii').split(",")
    openKnxId = uf2Version[0]
    appNumber = uf2Version[1]
    appVersion = uf2Version[2]
    appRevision = uf2Version[3]
    print("Id: " + openKnxId + ", Number: " + appNumber + ", Version: " + appVersion + ", Revision: " + appRevision)
    with open(source[0].get_path()[0:-4] + ".uf2", "rb") as orig_file:
        barray=bytearray(orig_file.read())
        barray[9] = barray[9] | 0x80
        barray[288] = 8 #Tag Size
        barray[289] = ord('K') #Type
        barray[290] = ord('N') #Type
        barray[291] = ord('X') #Type
        barray[292] = int(openKnxId) #Data
        barray[293] = int(appNumber) #Data
        barray[294] = int(appVersion) #Data
        barray[295] = int(appRevision) #Data
        with open(source[0].get_path()[0:-4] + "_new.uf2","wb") as file: 
            file.write(barray)

env.AddPostAction("buildprog", post_program_uf2)
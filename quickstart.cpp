#include "stdlib.h"
#include <sys/stat.h> 
#include <iostream>



const char* userHomeDir = std::getenv("HOME");
const char* nodeDirName = "test-node";
const char* algorandDataDirName = "data";
const char* updateScriptFileName = "update.sh";

const char* getEnvVar(const char* envVarToGet) {
    char* zshVar = std::getenv(envVarToGet);
    return zshVar;
}

int setupDirStructure (char* nodeDirPath) {
    /*
    * check if the path exists. If it does, return 0. If it does not,
    * create the directory, set it to 777 so that everyone has RWX
    * permissions, and return from the function.
    * 
    * @param nodeDirPath -> the path to create (if it does not already exist).
    * @return 0 (success).
    */

    // store stat() info in a struct
    struct stat info;
    // we can use dot-notation to access information about the folder/file
    // https://linux.die.net/man/2/stat
    // similar to running stat test-node in command-line

    if ( stat(nodeDirPath, &info) == -1 ) {
        // this means stat() was not able to contact the folder
        // since it does not exist, we create it
        printf("Test Node directory does not exist, creating it now...\n");

        int dirResponse = mkdir(nodeDirPath, 0777);
        return dirResponse;
    }
    else {
        //  this means stat() was able to contact the folder
        // since it already exists, we skip and return.
        printf("Test Node directory already exists, skipping...\n");
        return 0;
    };
}

char* setEnvVar(const char* varName, char* value) {
    /*
    * set environment variable where `varName`=`value`.
    * 
    * @param varName -> the name of the environment variable.
    * @param value -> the value to assign to `varName`.
    * 
    * @return -> `value`, the value assigned to `varName`.
    */

    setenv(varName, value, true);

    return value;
}

void downloadFile(const char* urlToDownload, char* downloadLocation){
    /*
    * download a file from `urlToDownload` into `downloadLocation`.
    * 
    * @param urlToDownload -> must be a valid URL to download from.
    * @param downloadLocation -> must be a folder that exists.
    * 
    * @return none.
    */
    char* command;
    sprintf(command, "wget %s -P %s", urlToDownload, downloadLocation);

    system(command);
}

void changeFilePermissions() {
    /*
    * change the permissions of a file to everyone can RWX.
    * 
    * @param none.
    * @return none.
    */
    int result = chmod("/Users/danielmurphy/test-node/update.sh", S_IRWXU);
    printf("PERMISSIONS: %d", result);
}

int runUpdateScript() {
    /*
    * run the update.sh script
    * 
    * @param none
    * @return 0 (success)
    */
    system("cd /Users/danielmurphy/test-node && yes | ./update.sh -i -c stable -p ~/test-node -d ~/test-node/data -n");
    return 0;
}

int startNode() {
    /*
    * start the Algorand node by running `./goal node start -d data`.
    * 
    * @param none.
    * @return 0 (success).
    */
    system("cd /Users/danielmurphy/test-node && ./goal node start -d data");
    return 0;
};

int checkNodeStatus() {
    /*
    * check the status of the Algorand node with `./goal node status -d data`.
    *
    * @param none.
    * @return 0 (success).
    */
    system("cd /Users/danielmurphy/test-node && ./goal node status -d data");
    return 0;
};

int main () {
    std::cout << "Executing Script..." << std::endl;
    
    char fullNodePath [50];
    char dataPath [50];
    char updateScriptPath [50];

    const char* algorandDataEnvVar = "ALGORAND_DATA";
    const char* updateScriptUrl = "https://raw.githubusercontent.com/algorand/go-algorand-doc/master/downloads/installers/update.sh";

    sprintf(fullNodePath, "%s/%s", userHomeDir, nodeDirName);
    sprintf(dataPath, "%s/%s", fullNodePath, algorandDataDirName);
    sprintf(updateScriptPath, "%s/%s", fullNodePath, updateScriptFileName);
    
    printf("Node Directory Path is %s\n", fullNodePath);
    setupDirStructure(fullNodePath);

    setEnvVar(algorandDataEnvVar, dataPath);
    printf("Algorand Data Directory: %s\n", getEnvVar("ALGORAND_DATA"));

    downloadFile(updateScriptUrl, fullNodePath);
    changeFilePermissions();
    runUpdateScript();
    startNode();
    checkNodeStatus();
}
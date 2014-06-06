#########################################################33
# @author: Sriram Mahavadi
# 

# Complete extlp project will be uploaded and overriden on the server mentioned

HOST=192.168.10.47
USER=MT2012160
PASSWORD=ramsri

# Logging into server
# ssh -l remoteuser remotehost
# ssh -l MT2012160 192.168.10.47

PROJECT_FOLDER=/home/harsha/Desktop/sriram_thesis/codebase/extlp/build/local/extlp
DESTINATION_FOLDER=/home/MT2012160/
EXCLUDE_FOLDER=build

# scp -r <file to upload> <username>@<hostname>:<destination path>
#scp -r $PROJECT_FOLDER ${USER}@${HOST}:${DESTINATION_FOLDER}

# Copying the inputs - ONly once is sufficient
#scp -r /home/harsha/Desktop/sriram_thesis/miplib2010-1.0.4-complete.zip MT2012160@192.168.10.47:/home/MT2012160/inputs
#scp -r /media/New Volume/sriram_thesis/miplib2010_complete.tar.gz MT2012160@192.168.10.47:/home/MT2012160/inputs

scp ${PROJECT_FOLDER} ${USER}@${HOST}:${DESTINATION_FOLDER}
# rsync -av --exclude '*.svn' user@server:/my/dir .
#rsync -av --exclude "${EXCLUDE_FOLDER}" ${USER}@${HOST}:${DESTINATION_FOLDER}
echo "Uploaded ${PROJECT_FOLDER} Successfully..."
#rsync -r -a -v --exclude "${EXCLUDE_FOLDER}"-e "ssh -l ${USER}" --delete ${PROJECT_FOLDER} ${HOST}:${DESTINATION_FOLDER}

#rsync -r -a -v --exclude "build" --exclude ".git" -e "ssh -l MT2012160" --delete /home/harsha/Desktop/sriram_thesis/codebase/extlp 192.168.10.47:/home/MT2012160/codebase

#rsync -r -a -v --exclude "fun1" -e "ssh -l MT2012160" --delete /home/harsha/Desktop/sriram_thesis/codebase/temp_check 192.168.10.47:/home/MT2012160/just_rsync

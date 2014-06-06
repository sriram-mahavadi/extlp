#################################################################
##################################################################
# @author = Sriram Mahavadi
#
# Standardizes an LP by dumbly converting = and >= constraints to <= constraints
# Ignores this conversion for a file if already negative RHS exists

INPUT_DIR="/media/New Volume/sriram_thesis/miplib2010"
OUTPUT_MAX_DIR="${INPUT_DIR}_max"
OUTPUT_MIN_DIR="${INPUT_DIR}_min"
SAVEIFS=$IFS
IFS=$(echo -en "\n\b")
mkdir ${OUTPUT_MAX_DIR} ${OUTPUT_MIN_DIR}

#INPUT_FILES="${INPUT_DIR}/buildingenergy.mps"
INPUT_FILES="${INPUT_DIR}/buildingenergy.mps"
for f in $INPUT_FILES
do
        FILE_NAME=`basename ${f}`
	printf "%-30s:" ${FILE_NAME}
	# Check if problem has ENDATA for validation
	if [ $(grep -c "^ENDATA" ${f}) -eq 0 ]
	then
		printf "%-30s:\n" 'Ign: Has no ENDATA'
		exit
	fi
	RHS_START=$(grep -n "^RHS" ${f} | cut -d: -f1)
        FILE_END=$(wc -l ${f} | cut -f1 -d' ')
        FILE_END=$(($FILE_END - 1))

	RHS_END=${RHS_START}
	if [ $(grep -c "^RANGES" ${f}) -ne 0 ]
	then
                RHS_END=$(grep -n "^RANGES" ${f} | cut -d: -f1)
                if [ $(sed -n "${RHS_START},${RHS_END}p" ${f}|grep -c '-') -ne 0 ]
                then
                    printf "%-30s:\n" "Ign: Negative RHS"
                    continue
                fi
                sed "s/^ E/ L/g" ${f} | sed "s/^ G/ L/g" | sed "${RHS_END},${FILE_END}d" > "${OUTPUT_MIN_DIR}/${FILE_NAME}"
		printf "%-30s:\n" "Successful [RANGES]"
                #echo "RANGES Present: ${RHS_END}"
	elif [ $(grep -c "^BOUNDS" ${f}) -ne 0 ]
	then
		RHS_END=$(grep -n "^BOUNDS" ${f} | cut -d: -f1)
                if [ $(sed -n "${RHS_START},${RHS_END}p" ${f}|grep -c '-') -ne 0 ]
                then
                    printf "%-30s:\n" "Ign: Negative RHS"
                    continue
                fi
                sed "s/^ E/ L/g" ${f} | sed "s/^ G/ L/g" | sed "${RHS_END},${FILE_END}d" > "${OUTPUT_MIN_DIR}/${FILE_NAME}"		
                printf "%-30s:\n" "Successful [BOUNDS]"
                
                #echo "BOUNDS Present: ${RHS_END}"	
	else
		RHS_END=$(grep -n "^ENDATA" ${f} | cut -d: -f1)
                if [ $(sed -n "${RHS_START},${RHS_END}p" ${f}|grep -c '-') -ne 0 ]
                then
                    printf "%-30s:\n" "Ign: Negative RHS [ENDATA]"
                    continue
                fi
                sed "s/^ E/ L/g" ${f} | sed "s/^ G/ L/g" > "${OUTPUT_MIN_DIR}/${FILE_NAME}"
		printf "%-30s:\n" "Successful"
                #echo "ENDATA Present: ${RHS_END}"
	fi
	# Converting >= or = constraints to <=
	# sed "s/^ E/ L/g" ${f} | sed "s/^ G/ L/g"
done

IFS=$SAVEIFS


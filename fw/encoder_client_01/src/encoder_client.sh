#! /bin/bash
# Simulate Encoder client: epoch,status,run_time,total_counts,delta_counts,hz 
# send to stdout and to /mnt/tmp/encoder_client.status

EraseAndCreateNewEncoderStatusFile() {
encoder_client_file="/mnt/tmp/encoder_client.status"
if [ -f $encoder_client_file ]; then
  rm $encoder_client_file
  sleep 1
fi
touch $encoder_client_file
sleep 1
}

# -- Start script
run_time=0
counts=0
status=1; status_max=65536
EraseAndCreateNewEncoderStatusFile

# Infinite loop 
while (( 1 )); do 
  # Simulate: run_time, total counts and animate status
  epoch=$(date +%s)
  ((run_time+=5)); 
  ((++counts)); 
  if  ((status < status_max)); then 
    ((status <<= 1))
  else
    status=1
  fi

  # output 
  OUT=$(echo $epoch,$status,$run_time,$counts,4938,1234.5)
  # echo $OUT
  echo $OUT >> $encoder_client_file

  # Create a new file every 100 counts
  if (( ((counts % 100)) == 0 )); then
    EraseAndCreateNewEncoderStatusFile
  fi
  sleep 0.5 
done
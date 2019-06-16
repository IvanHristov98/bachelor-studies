#!/bin/bash

echo "Gathering data...";

beep_end=$(\
	cat ${1} | \
	egrep '^beep [0-9\.]+ [0-9\.]+$' | \
	sed -r 's/beep [0-9\.]+ ([0-9\.]+)/\1/g' \
);

eeg_start_recording_date=$(\
	cat ${2} | \
	egrep '^<StartRecordingDate>.*</StartRecordingDate>$' | \
	sed -r 's/^<StartRecordingDate>(.*)<\/StartRecordingDate>$/\1/g' | \
	sed -r 's/\./ /g' | \
	awk '{ print $3"-"$2"-"$1;}'\
);

eeg_start_recording_time=$( \
	cat ${2} | \
	egrep '^<StartRecordingTime>.*</StartRecordingTime>$' | \
	sed -r 's/^<StartRecordingTime>(.*)<\/StartRecordingTime>$/\1/g' \
);

eeg_sampling_rate=$(\
	cat ${2} | \
	egrep '^<SamplingRate>.*</SamplingRate>$' | \
	sed -r 's/^<SamplingRate>([0-9]+).*<\/SamplingRate>/\1/g' \
);

wav_length=$(\
	sox example/larynx.wav -n stat 2>&1 | \
	grep '^Length' | \
	sed -r 's/.*[[:space:]]+//g' \
);

echo "${3} length> ${wav_length}";
echo "Handling timezones...";

# if EEG_TZ available
if [ -n "${EEG_TZ}" ]; then
	eeg_start=$(( $(TZ="${EEG_TZ}" date -d "${eeg_start_recording_date} ${eeg_start_recording_time}" +%s%N) / 1000 ));
else
	# %s returns seconds from UNIX epoch in UTC by default
	eeg_start=$(( $(date -d "${eeg_start_recording_date} ${eeg_start_recording_time}" +%s%N) / 1000 ));
fi

eeg_start=$(echo "${eeg_start}" | sed -r 's/([0-9]+)([0-9]{6})$/\1.\2/g');

echo "Gathering tick data...";

num_ticks=$( \
	cat ${2} | \
	awk 'BEGIN {
		FS="%";
		isEegData=0;
	}
	{
		if ($1 == "</EEGData>"){
			isEegData=0;
		}

		if (isEegData == 1) {
			print $1;
		}

		if ($1 == "<EEGData>"){
			isEegData=1;
		}
	}' | wc -l
);

echo "Number of found ticks> ${num_ticks}";
echo "Checking for repetitive stimuluses...";

max_repeats=$( \
	cat ${1} | \
	sort | \
	sed -r 's/^([^ ]+) [0-9\.]+ [0-9\.]+$/\1/g' | \
	uniq -c | \
	sed -r 's/[[:space:]]*([0-9]*.*)/\1/g' | \
	sort -n -r | \
	head -n 1 | \
	cut -d ' ' -f 1 \
);

if [ $max_repeats -gt 1 ]; then
	echo "Repetitive stimuluses found. Program terminating...";
	kill -9 $$;
fi

stimulus_logs=$( \
	cat ${1} | \
	egrep -v '^beep [0-9\.]+ [0-9\.]+$'
);

wav_start_microsecs=$(echo "${beep_end}" | tr -d '.' );
wav_end_microsecs=$(( $wav_start_microsecs + $( \
	echo "${wav_length}" | \
	tr -d '.') \
));

eeg_start_microsecs=$(echo "${eeg_start}" | tr -d '.' );
eeg_tick_duration=$(echo $(( 1000000 / ${eeg_sampling_rate}  )));
eeg_end_microsecs=$(( ${eeg_start_microsecs} + ${eeg_tick_duration} * ${num_ticks} ));

# creating output directory
mkdir -p "${4}";

IFS='
';

echo "Initiating splitting procedure...";

for line in ${stimulus_logs}; do
	# start time in seconds * 1 000 000
	stimulus_start=$( \
		echo "${line}" | \
		sed -r 's/^[^ ]+ ([0-9\.]+) [0-9\.]+$/\1/g' | \
		sed -r 's/^([0-9]+)\.([0-9]{6})/\1\2/g' \
	);

	# end time in seconds * 1 000 000
	stimulus_end=$( \
		echo "${line}" | \
		sed -r 's/^[^ ]+ [0-9\.]+ ([0-9\.]+)$/\1/g' | \
		sed -r 's/^([0-9]+)\.([0-9]{6})/\1\2/g' \
	);

	# stimulus name
	stimulus=$( \
		echo "${line}" | \
		sed -r 's/^([^ ]+) [0-9\.]+ [0-9\.]+$/\1/g' \
	);

	if [ $(( ${stimulus_end} - ${stimulus_start} )) -lt 200000 ]; then
		echo "Stimulus ${stimulus} period shorter than 0.2 seconds.";
		continue;
	fi

	# wav file margins
	if [ ${stimulus_start} -lt ${wav_start_microsecs} ]; then
		echo "Stimulus ${stimulus} period begins before .wav file.";
		continue;
	fi

	if [ ${stimulus_end} -gt ${wav_end_microsecs} ]; then
		echo "Stimulus ${stimulus} period outgoes the .wav file length.";
		continue;
	fi

	# xml file margins
	if [ ${stimulus_start} -lt ${eeg_start_microsecs} ]; then
		echo "Stimulus ${stimulus} period begins before .xml file.";
		continue;
	fi

	if [ ${stimulus_end} -gt ${eeg_end_microsecs} ]; then
		echo "Stimulus ${stimulus} period outgoes the .xml file.";
		continue;
	fi

	# cutting wav file
	start_trim=$(( ${stimulus_start} - ${wav_start_microsecs} ));
	# converting to seconds
	start_trim=$( \
		echo "${start_trim}" | \
		sed -r 's/([0-9]*)([0-9]{6})$/\1\.\2/g' \
	);

	stimuli_length=$(( ${stimulus_end} - ${stimulus_start} ));

	# converting to seconds
	stimuli_length=$( \
		echo "${stimuli_length}" | \
		sed -r 's/([0-9]*)([0-9]{6})$/\1\.\2/g' \
	);

	sox "${3}" "${4}/${stimulus}_lar.wav" trim ${start_trim} ${stimuli_length};

	# cutting ticks
	start_pos=$(( $(( ${stimulus_start} - ${eeg_start_microsecs} )) / ${eeg_tick_duration} ));

	stimuli_length_in_positions=$(( $(( ${stimulus_end} - ${stimulus_start} )) / ${eeg_tick_duration} ));

	touch -f "${4}/${stimulus}_eeg.xml";

	cat ${2} | \
	awk 'BEGIN {
		FS="%";
		dataStarted=0;
	}
	{
		if (dataStarted == 1) {
			print $1;
		}

		if ($1 == "<EEGData>") {
			dataStarted=1;
		}
	}' | \
	tail -n +${start_pos} | \
	head -n ${stimuli_length_in_positions} 1> "${4}/${stimulus}_eeg.xml";
done
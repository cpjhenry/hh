#!/bin/bash
# Hanke-Henry Permanent Calendar Date Converter
# Initial 2015-03-21/pjh
# Rewrite 2018-08-06/pjh

# See: http://hankehenryontime.com/ for details.

# Calculates H-H date (in UTC), based on current date, then solves for edge cases.

# TODO: Calculate dates past 2038.

VERBOSE=
GREGORY=
PREFIX=
DISPLAY=true
DISTIME=
DISPDOW=
highlit=true

DOMINI=(G A B C D E F)
MONTHS=(- Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec Xtr)
MONTHL=(- January February March April May June July August September October November December Xtr)
DOWS=(Sun Mon Tue Wed Thu Fri Sat)
DOWL=(Sunday Monday Tuesday Wednesday Thursday Friday Saturday)

HHm=(0\
	1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\
	2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2\
	3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3\
	4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4\
	5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5\
	6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6 6\
	7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7\
	8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8\
	9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9\
	10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10\
	11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11\
	12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12 12\
	13 13 13 13 13 13 13\
	)
HHd=(0\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\
	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31\
	1 2 3 4 5 6 7\
	)

setdate() { # usage: setdate [yesterday|today|tomorrow]
	# UTC
	ctime=$(date +%R)
	utime=$(date -u +%R)

	year=$(date -ud "$1 $ctime" +%Y)
	month=$(date -ud "$1 $ctime" +%0m)
	day=$(date -ud "$1 $ctime" +%0d)
	dow=$(date -ud "$1 $ctime" +%w)
}

fixUTC() {
	if [ "$(date -u +%H)" -lt "$(date +%H)" ]; then
		setdate "$year-$month-$day + 1 day"
	fi
}

is_leap_year() { ## USAGE: is_leap_year [year]
	ily_year=${1:-`date +%Y`}
	case $ily_year in
		*0[48] |\
		*[2468][048] |\
		*[13579][26] |\
		*[02468][048]00 |\
		*[13579][26]00 ) _IS_LEAP_YEAR=1
		return 0 ;;
		*) unset _IS_LEAP_YEAR
		return 1 ;;
	esac
}

is_extra() {
	for i in 2020 2026 2032 2037 2043 2048 2054; do
		if [ "$1" == "$i" ]; then
			xtr=true
			xtr_text="X"
		fi
		if [ "$(( $1-1 ))" == "$i" ]; then
			pxtr=true
		fi
	done
}

print_calendar() {
	if [ $highlit ]; then
		bo=$'\e[1m'
		no=$'\e[0m'
	fi
	cat <<EOF | expand -t4
								Hanke-Henry Permanent Calendar (HHPC)
																								$bo
	January							February						March
	Mo	Tu	We	Th	Fr	Sa	Su		Mo	Tu	We	Th	Fr	Sa	Su		Mo	Tu	We	Th	Fr	Sa	Su	$no
	01	02	03	04	05	06	07				01	02	03	04	05						01	02	03
	08	09	10	11	12	13	14		06	07	08	09	10	11	12		04	05	06	07	8	9	10
	15	16	17	18	19	20	21		13	14	15	16	17	18	19		11	12	13	14	15	16	17
	22	23	24	25	26	27	28		20	21	22	23	24	25	26		18	19	20	21	22	23	24
	29	30							27	28	29	30					25	26	27	28	29	30	31
	--------------------------		--------------------------		--------------------------	$bo
	April							May								June
	Mo	Tu	We	Th	Fr	Sa	Su		Mo	Tu	We	Th	Fr	Sa	Su		Mo	Tu	We	Th	Fr	Sa	Su	$no
	01	02	03	04	05	06	07				01	02	03	04	05						01	02	03
	08	09	10	11	12	13	14		06	07	08	09	10	11	12		04	05	06	07	08	09	10
	15	16	17	18	19	20	21		13	14	15	16	17	18	19		11	12	13	14	15	16	17
	22	23	24	25	26	27	28		20	21	22	23	24	25	26		18	19	20	21	22	23	24
	29	30							27	28	29	30					25	26	27	28	29	30	31
	--------------------------		--------------------------		--------------------------	$bo
	July							August							September
	Mo	Tu	We	Th	Fr	Sa	Su		Mo	Tu	We	Th	Fr	Sa	Su		Mo	Tu	We	Th	Fr	Sa	Su	$no
	01	02	03	04	05	06	07				01	02	03	04	05						01	02	03
	08	09	10	11	12	13	14		06	07	08	09	10	11	12		04	05	06	07	08	09	10
	15	16	17	18	19	20	21		13	14	15	16	17	18	19		11	12	13	14	15	16	17
	22	23	24	25	26	27	28		20	21	22	23	24	25	26		18	19	20	21	22	23	24
	29	30							27	28	29	30					25	26	27	28	29	30	31
	--------------------------		--------------------------		--------------------------	$bo
	October							November						December
	Mo	Tu	We	Th	Fr	Sa	Su		Mo	Tu	We	Th	Fr	Sa	Su		Mo	Tu	We	Th	Fr	Sa	Su	$no
	01	02	03	04	05	06	07				01	02	03	04	05						01	02	03
	08	09	10	11	12	13	14		06	07	08	09	10	11	12		04	05	06	07	08	09	10
	15	16	17	18	19	20	21		13	14	15	16	17	18	19		11	12	13	14	15	16	17
	22	23	24	25	26	27	28		20	21	22	23	24	25	26		18	19	20	21	22	23	24
	29	30							27	28	29	30					25	26	27	28	29	30	31
	--------------------------		--------------------------		--------------------------	$bo
																	Xtra
																	Mo	Tu	We	Th	Fr	Sa	Su	$no
																	01	02	03	04	05	06	07
EOF
}

setdate today

while getopts "gpstwy:m:d:lcHv?" OPTION
do
	case $OPTION in
		g)	[ $GREGORY ] && unset GREGORY || GREGORY=true;;	# Gregorian date
		p)	[ $PREFIX  ] && unset PREFIX  || PREFIX=true;;	# prefix
		s)	[ $DISPLAY ] && unset DISPLAY || DISPLAY=true;;	# H-H date
		t)	[ $DISTIME ] && unset DISTIME || DISTIME=true;;	# time display
		w)	[ $DISPDOW ] && unset DISPDOW || DISPDOW=true;;	# day-of-week

		y)	setdate "$OPTARG-$month-$day ";;				# set year
		m)	setdate "$year-$OPTARG-$day  ";;				# set month
		d)	setdate "$year-$month-$OPTARG"; fixUTC;;		# set day

		l)	for i in {2018..2054}; do						# list dominical letters
			echo $i $(hh -vs -y$i -m1 -d1); done; exit;;
		c)	print_calendar; exit;;							# print calendar
		H)	unset highlit;;									# disable highlighting
		v)	VERBOSE=true;;									# verbose
		*)	[ $(which getusage) ] && getusage $0; exit;;
	esac
done

# Hanke-Henry calendar adopted 1 January 2018
[[ $year -lt 2018 ]] && echo $(basename $0): Proleptic display not enabled. && exit

# Set HH date
jd="$(date -d $year-$month-$day +%j)"
soy="$(date -d $year-01-01 +%w)"
eoy="$(date -d $year-12-31 +%w)"

(( dominical=((year-1)%100%4*2+(year-1)%100%7*4+(year-1)/100%4*2)%7 ))

is_leap_year $year
if [ $_IS_LEAP_YEAR ]; then
	leap="L"
	if [ $dominical -eq 0 ]; then 
		dom="${DOMINI[$dominical]}${DOMINI[6]}"
	else
		dom="${DOMINI[$dominical]}${DOMINI[$dominical-1]}"
	fi
else
	leap=""
	dom="${DOMINI[$dominical]}"
fi

is_extra $year

# Solve for edge cases
case $dom in
	F)	(( jd=$jd+1 ))
		[ "$month-$day" == "12-30" ] && jd=1 && (( year=$year+1 )) && month=1 && day=1
		[ "$month-$day" == "12-31" ] && jd=2 && (( year=$year+1 )) && month=1 && day=2;;
	G)	[ "$month-$day" == "12-31" ] && jd=1 && (( year=$year+1 )) && month=1 && day=1;;
	ED)	(( jd=$jd+2 ));;
	C)	(( jd=$jd-3 ))
		[ "$month-$day" == "01-01" ] && jd=369 && (( year=$year-1 ))
		[ "$month-$day" == "01-02" ] && jd=370 && (( year=$year-1 ))
		[ "$month-$day" == "01-03" ] && jd=371 && (( year=$year-1 ));;
	B)	(( jd=$jd-2 ))
		if [ $pxtr ]; then # previous year is xtr year
			[ "$month-$day" == "01-01" ] && jd=370 && (( year=$year-1 ))
			[ "$month-$day" == "01-02" ] && jd=371 && (( year=$year-1 ))
		else
			[ "$month-$day" == "01-01" ] && jd=363 && (( year=$year-1 ))
			[ "$month-$day" == "01-02" ] && jd=364 && (( year=$year-1 ))
		fi;;
	A)	(( jd=$jd-1 ))
		[ "$month-$day" == "01-01" ] && jd=364 && (( year=$year-1 ));;
	GF)	[ "$month-$day" == "12-30" ] && jd=1 && (( year=$year+1 )) && month=1 && day=1
		[ "$month-$day" == "12-31" ] && jd=2 && (( year=$year+1 )) && month=1 && day=2;;
	E)	(( jd=$jd+2 ))
		[ "$month-$day" == "12-29" ] && jd=1 && (( year=$year+1 )) && month=1 && day=1
		[ "$month-$day" == "12-30" ] && jd=2 && (( year=$year+1 )) && month=1 && day=2
		[ "$month-$day" == "12-31" ] && jd=3 && (( year=$year+1 )) && month=1 && day=3;;
	D)	(( jd=$jd+3 ));;
	BA)	(( jd=$jd-2 ))
		[ "$month-$day" == "01-01" ] && jd=363 && (( year=$year-1 ))
		[ "$month-$day" == "01-02" ] && jd=364 && (( year=$year-1 ));;
	DC)	(( jd=$jd+3 ));;
	FE)	(( jd=$jd+1 ))
		[ "$month-$day" == "12-29" ] && jd=1 && (( year=$year+1 )) && month=1 && day=1
		[ "$month-$day" == "12-30" ] && jd=2 && (( year=$year+1 )) && month=1 && day=2
		[ "$month-$day" == "12-31" ] && jd=3 && (( year=$year+1 )) && month=1 && day=3;;
esac

[ $VERBOSE ] && printf "DOM $dom -> starts ${DOWL[$soy]}, ends ${DOWL[$eoy]} $leap$xtr_text\n"
[ $GREGORY ] && [ $PREFIX  ] && printf "Grg. "
[ $GREGORY ] && [ $DISPDOW ] && printf "%s " ${DOWL[$dow]}
[ $GREGORY ] && printf "%d %s %d UTC\n" $day ${MONTHL[10#$month]} $year
[ $DISPLAY ] && [ $PREFIX  ] && printf "H-H. "
[ $DISPLAY ] && [ $DISPDOW ] && printf "%s " ${DOWL[$dow]}
[ $DISPLAY ] && printf "%s %s %s" ${HHd[10#$jd]} ${MONTHL[${HHm[10#$jd]}]} $year
[ $DISPLAY ] && [ $DISTIME ] && printf " %s"  $utime
[ $DISPLAY ] && printf "\n"

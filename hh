#!/bin/bash
# Hanke-Henry Permanent Calendar Date Converter
# Initial 2015-03-21/pjh
# Rewrite 2018-08-06/pjh
# Fix UTC 2019-08-30/pjh

# See: http://hankehenryontime.com/ for details.

# Calculates H-H date (in UTC), based on current Gregorian date, then solves
# for edge cases.

# TODO Verify dates past 2038

DISPLAY=true
GREGORY=
PREFIX=
DISTIME=
DISPDOW=

VERBOSE=
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
	year=$(	date -d "$1" $2 +%Y)
	month=$(date -d "$1" $2 +%0m)
	day=$(	date -d "$1" $2 +%0d)
	dow=$(	date -d "$1" $2 +%w)
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
centre() {
  termwidth=20
  padding="$(printf '%0.1s' ' '{1..500})"
  printf "%*.*s $bo%s$no %*.*s\n" 0 "$(((termwidth-2-${#1})/2))" "$padding" "$1" 0 "$(((termwidth-1-${#1})/2))" "$padding"
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
	08	09	10	11	12	13	14		06	07	08	09	10	11	12		04	05	06	07	08	09	10
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
print_month() {
	local mon

	if [ $highlit ]; then
		bo=$'\e[1m'
		no=$'\e[0m'; fi

	(( mon=10#$month ))
	[[ $mon == 1 || $mon == 4 || $mon == 7 || $mon == 10 ]] && pos=1
	[[ $mon == 2 || $mon == 5 || $mon == 8 || $mon == 11 ]] && pos=2
	[[ $mon == 3 || $mon == 6 || $mon == 9 || $mon == 12 ]] && pos=3

	[ $VERBOSE ] && printf "This month: %s, position %s\n" $mon $pos
	centre "${MONTHL[$mon]}"
	
	case $pos in
		1)  echo -n "$bo"
			echo "Mo Tu We Th Fr Sa Su $no"
			echo "01 02 03 04 05 06 07"
			echo "08 09 10 11 12 13 14"
			echo "15 16 17 18 19 20 21"
			echo "22 23 24 25 26 27 28"
			echo "29 30"
		;;
		2)  echo -n "$bo"
			echo "Mo Tu We Th Fr Sa Su $no"
			echo "      01 02 03 04 05"
			echo "06 07 08 09 10 11 12"
			echo "13 14 15 16 17 18 19"
			echo "20 21 22 23 24 25 26"
			echo "27 28 29 30"
		;;
		3)  echo -n "$bo"
			echo "Mo Tu We Th Fr Sa Su $no"
			echo "            01 02 03"
			echo "04 05 06 07 08 09 10"
			echo "11 12 13 14 15 16 17"
			echo "18 19 20 21 22 23 24"
			echo "25 26 27 28 29 30 31"
		;;
	esac

}

setdate today

while getopts "ghptwWy:m:d:lLCMHUv?" OPTION
do
	case $OPTION in
		g)	[ $GREGORY ] && unset GREGORY || GREGORY=true;;	# Gregorian date
		h)	[ $DISPLAY ] && unset DISPLAY || DISPLAY=true;;	# Hanke-Henry date
		p)	[ $PREFIX  ] && unset PREFIX  || PREFIX=true;;	# prefix
		t)	[ $DISTIME ] && unset DISTIME || DISTIME=true;;	# time display
		w)	[ $DISPDOW ] && unset DISPDOW || DISPDOW=true;;	# day-of-week
		W)	[ $DISPDWS ] && unset DISPDWS || DISPDWS=true;;	# short day-of-week

		y)	year=$OPTARG;;									# set year
		m)	month=$OPTARG;;									# set month
		d) 	day=$OPTARG;;									# set day

		l)	for i in {2018..2054}; do						# list dominical letters
			echo $i $(hh -hL -y$i -m1 -d1); done; exit;;
		L)	DOMINIC=true;;
		C)	print_calendar; exit;;							# print calendar
		M)	print_month;exit;;								# print monthly calendar
		H)	unset highlit;;									# disable highlighting
		U)	cp -uv $HOME/bin/hh $HOME/src/hh/				# update git repository
			cp -uv $HOME/share/man/man1/hh.1 $HOME/src/hh/
			exit;;
		v)	VERBOSE=true;;									# verbose
		*)	[ $(which getusage) ] && getusage $0; exit;;
	esac
done

setdate $year-$month-$day
# fixUTC
ctime=$(date +%R)
utime=$(date -u +%R)
tz=$(date -d "$year-$month-$day" +%Z)
[ $VERBOSE ] && echo "$year-$month-$day $ctime $tz"

setdate "$year-$month-$day $ctime $tz" "-u"
[ $VERBOSE ] && echo "$year-$month-$day $utime UTC"

# Hanke-Henry calendar adopted 1 January 2018
[[ $year -lt 2018 ]] && echo $(basename $0): Proleptic display not enabled. && exit

# Set HH date
doy="$(date -d $year-$month-$day +%j)"
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
	F)	(( doy=10#$doy+1 ))
		[ "$month-$day" == "12-30" ] && doy=1 && (( year=$year+1 )) && month=1 && day=1
		[ "$month-$day" == "12-31" ] && doy=2 && (( year=$year+1 )) && month=1 && day=2;;
	G)	[ "$month-$day" == "12-31" ] && doy=1 && (( year=$year+1 )) && month=1 && day=1;;
	ED)	(( doy=10#$doy+2 ));;
	C)	(( doy=10#$doy-3 ))
		[ "$month-$day" == "01-01" ] && doy=369 && (( year=$year-1 ))
		[ "$month-$day" == "01-02" ] && doy=370 && (( year=$year-1 ))
		[ "$month-$day" == "01-03" ] && doy=371 && (( year=$year-1 ));;
	B)	(( doy=10#$doy-2 ))
		if [ $pxtr ]; then # previous year is xtr year
			[ "$month-$day" == "01-01" ] && doy=370 && (( year=$year-1 ))
			[ "$month-$day" == "01-02" ] && doy=371 && (( year=$year-1 ))
		else
			[ "$month-$day" == "01-01" ] && doy=363 && (( year=$year-1 ))
			[ "$month-$day" == "01-02" ] && doy=364 && (( year=$year-1 ))
		fi;;
	A)	(( doy=10#$doy-1 ))
		[ "$month-$day" == "01-01" ] && doy=364 && (( year=$year-1 ));;
	GF)	[ "$month-$day" == "12-30" ] && doy=1 && (( year=$year+1 )) && month=1 && day=1
		[ "$month-$day" == "12-31" ] && doy=2 && (( year=$year+1 )) && month=1 && day=2;;
	E)	(( doy=10#$doy+2 ))
		[ "$month-$day" == "12-29" ] && doy=1 && (( year=$year+1 )) && month=1 && day=1
		[ "$month-$day" == "12-30" ] && doy=2 && (( year=$year+1 )) && month=1 && day=2
		[ "$month-$day" == "12-31" ] && doy=3 && (( year=$year+1 )) && month=1 && day=3;;
	D)	(( doy=10#$doy+3 ));;
	BA)	(( doy=10#$doy-2 ))
		[ "$month-$day" == "01-01" ] && doy=363 && (( year=$year-1 ))
		[ "$month-$day" == "01-02" ] && doy=364 && (( year=$year-1 ));;
	DC)	(( doy=10#$doy+3 ));;
	FE)	(( doy=10#$doy+1 ))
		[ "$month-$day" == "12-29" ] && doy=1 && (( year=$year+1 )) && month=1 && day=1
		[ "$month-$day" == "12-30" ] && doy=2 && (( year=$year+1 )) && month=1 && day=2
		[ "$month-$day" == "12-31" ] && doy=3 && (( year=$year+1 )) && month=1 && day=3;;
esac

[ $DOMINIC ] && printf "DOM $dom -> starts ${DOWL[$soy]}, ends ${DOWL[$eoy]} $leap$xtr_text\n"

[ $GREGORY ] && [ $PREFIX  ] && printf "Grg. "
[ $GREGORY ] && [ $DISPDOW ] && printf "%s " ${DOWL[$dow]}
[ $GREGORY ] && printf "%d %s %d UTC\n" $day ${MONTHL[10#$month]} $year

[ $DISPLAY ] && [ $PREFIX  ] && printf "H-H. "
[ $DISPLAY ] && [ $DISPDOW ] && printf "%s " ${DOWL[$dow]}
[ $DISPLAY ] && [ $DISPDWS ] && printf "%s " ${DOWS[$dow]}
[ $DISPLAY ] && printf "%s %s %s" ${HHd[10#$doy]} ${MONTHL[${HHm[10#$doy]}]} $year
[ $DISPLAY ] && [ $DISTIME ] && printf " %s"  $utime
[ $DISPLAY ] && printf "\n"

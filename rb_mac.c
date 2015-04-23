/*
**
** Copyright (c) 2014, Eneo Tecnologia
** Author: Eugenio Perez <eupm90@gmail.com>
** All rights reserved.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "rb_mac.h"
#include <string.h>

static int ishexchar(const char x) {
	return ('0' <= x && x <= '9') || ('a' <= x && x <= 'f') || ('A' <= x && x <= 'F');
}

static uint64_t hexchar(const unsigned x) {
	return ('0' <= x && x <= '9') ? x - '0' : 
	       ('a' <= x && x <= 'f') ? x - 'a' + 10 :
	       x - 'A' +10;
}

static int validmac(const char *a) {
	return 
		ishexchar(a[ 0]) && ishexchar(a[ 1]) &&
		ishexchar(a[ 3]) && ishexchar(a[ 4]) &&
		ishexchar(a[ 6]) && ishexchar(a[ 7]) &&
		ishexchar(a[ 9]) && ishexchar(a[10]) &&
		ishexchar(a[12]) && ishexchar(a[13]) &&
		ishexchar(a[15]) && ishexchar(a[16]) &&
		a[2]  == ':' && a[5] == ':' && a[8] == ':' &&
		a[11] == ':' && a[14] == ':';
}

uint64_t parse_mac(const char *_mac) {
	if(strlen(_mac) != strlen("00:00:00:00:00:00"))
		return 0xFFFFFFFFFFFFFFFFL;

	if(!validmac(_mac))
		return 0xFFFFFFFFFFFFFFFFL;

	const unsigned char *mac = (const unsigned char *)_mac;

	return 0L + 
		(hexchar(mac[16])<<0)+
		(hexchar(mac[15])<<4)+
		(hexchar(mac[13])<<8)+
		(hexchar(mac[12])<<12)+
		(hexchar(mac[10])<<16)+
		(hexchar(mac[ 9])<<20)+
		(hexchar(mac[ 7])<<24)+
		(hexchar(mac[ 6])<<28)+
		(hexchar(mac[ 4])<<32)+
		(hexchar(mac[ 3])<<36)+
		(hexchar(mac[ 1])<<40)+
		(hexchar(mac[ 0])<<44);
}

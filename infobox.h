/* SCCS-info %W% %E% */

/*--------------------------------------------------------------------*/
/*								      */
/*		VCG : Visualization of Compiler Graphs		      */ 
/*		--------------------------------------		      */ 
/*								      */
/*   file:	   infobox.h					      */
/*   version:	   1.00.00					      */
/*   creation:	   23.12.94				      	      */
/*   author:	   I. Lemke  (...-Version 0.99.99)		      */ 
/*		   G. Sander (Version 1.00.00-...)		      */ 
/*		   Universitaet des Saarlandes, 66041 Saarbruecken    */
/*		   ESPRIT Project #5399 Compare 		      */ 
/*   description:  Info and label box drawing 			      */ 
/*   status:	   in work					      */
/*								      */
/*--------------------------------------------------------------------*/


/* $Id: infobox.h,v 1.2 1995/02/08 11:11:14 sander Exp $ */

/*
 *   Copyright (C) 1993--1995 by Georg Sander, Iris Lemke, and
 *                               the Compare Consortium 
 *
 *  This program and documentation is free software; you can redistribute 
 *  it under the terms of the  GNU General Public License as published by
 *  the  Free Software Foundation;  either version 2  of the License,  or
 *  (at your option) any later version.
 *
 *  This  program  is  distributed  in  the hope that it will be useful,
 *  but  WITHOUT ANY WARRANTY;  without  even  the  implied  warranty of
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the
 *  GNU General Public License for more details.
 *
 *  You  should  have  received a copy of the GNU General Public License
 *  along  with  this  program;  if  not,  write  to  the  Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  The software is available per anonymous ftp at ftp.cs.uni-sb.de.
 *  Contact  sander@cs.uni-sb.de  for additional information.
 */


/* 
 * $Log: infobox.h,v $
 * Revision 1.2  1995/02/08  11:11:14  sander
 * Distribution version 1.3.
 *
 * Revision 1.1  1994/12/23  18:12:45  sander
 * Initial revision
 *
 */

#ifndef INFOBOX_H
#define INFOBOX_H

/*--------------------------------------------------------------------*/

/* See infobox.c for explanation
 * -----------------------------
 */


/*  Prototypes
 *  ---------- 
 */

void gs_infobox	_PP((GNODE v,long x, long y, int a,int b,int info));
void free_infoboxes	     _PP((void));
int search_and_close_infobox _PP((long x, long y));
void draw_infoboxes	     _PP((void));

/*--------------------------------------------------------------------*/

#endif /* INFOBOX_H */



/***********************************************************************
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  $Author$
 *
 *  --------------------------------------------------------------------
 *  Company:        N&K Development, all rights reserved
 *
 *                  Neugebauer & Krizsan Cooperation, Germany
 *                  contact:    arnold.neugebauer@web.de
 *                              ralf.krizsan@web.de
 *  --------------------------------------------------------------------
 *  Created:        25-10-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         standard lib
 *  Purpose:        standard functions for number to ascii type conversions
 *  Comments:       -
 *
 ************************************************************************ */


/***
*char *_itoa, *_ltoa, *_ultoa(val, buf, radix) - convert binary int to ASCII
*       string
*
*Purpose:
*       Converts an int to a character string.
*
*Entry:
*       val - number to be converted (int, long or unsigned long)
*       int radix - base to convert into
*       char *buf - ptr to buffer to place result
*
*Exit:
*       fills in space pointed to by buf with string result
*       returns a pointer to this buffer
*
*Exceptions:
*
*******************************************************************************/

/* helper routine that does the main job. */

static void xtoa (
        unsigned long val,
        char far* buf,
        unsigned radix,
        int is_neg
        )
{
        char far* p;                /* pointer to traverse string */
        char far* firstdig;         /* pointer to first digit */
        char temp;              /* temp char */
        unsigned digval;        /* value of digit */

        p = buf;

        if (is_neg) {
            /* negative, so output '-' and negate */
            *p++ = '-';
            val = (unsigned long)(-(long)val);
        }

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (char) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (char) (digval + '0');       /* a digit */
        } while (val > 0);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = '\0';            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}

/***********************************************************************
 *        FUNCTION:       itoa()
 *        DESCRIPTION:    convert integer to ascii string
 *        PARAMETER:      int val       number to be converted 
 *                        int radix     base to convert into
 *                        char *buf     ptr to buffer to place result
 *        RETURN:         ptr to buffer to place result
 *        COMMENT:        Actual functions just call conversion helper 
 *                        with neg flag set correctly, and return pointer 
 *                        to buffer. 
 *********************************************************************** */  
char far* itoa ( int val, char far* buf, int radix)
{
        if (radix == 10 && val < 0)
            xtoa((unsigned long)val, buf, radix, 1);
        else
            xtoa((unsigned long)(unsigned int)val, buf, radix, 0);
        return buf;
}

/***********************************************************************
 *        FUNCTION:       ltoa()
 *        DESCRIPTION:    convert long to ascii string
 *        PARAMETER:      long val       number to be converted 
 *                        int radix     base to convert into
 *                        char *buf     ptr to buffer to place result
 *        RETURN:         ptr to buffer to place result
 *        COMMENT:        Actual functions just call conversion helper 
 *                        with neg flag set correctly, and return pointer 
 *                        to buffer. 
 *********************************************************************** */  
char far* ltoa ( long val, char far* buf, int radix )
{
        xtoa((unsigned long)val, buf, radix, (radix == 10 && val < 0));
        return buf;
}

/***********************************************************************
 *        FUNCTION:       ultoa()
 *        DESCRIPTION:    convert unsigned long to ascii string
 *        PARAMETER:      unsigned long val     number to be converted 
 *                        int radix             base to convert into
 *                        char *buf             ptr to buffer to place result
 *        RETURN:         ptr to buffer to place result
 *        COMMENT:        Actual functions just call conversion helper 
 *                        with neg flag set correctly, and return pointer 
 *                        to buffer. 
 *********************************************************************** */  
char far* ultoa ( unsigned long val, char far* buf, int radix )
{
        xtoa(val, buf, radix, 0);
        return buf;
}


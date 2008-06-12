/*
libdmtx - Data Matrix Encoding/Decoding Library

Copyright (c) 2008 Mike Laughton

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact: mike@dragonflylogic.com
*/

/* $Id$ */

/**
 * @file dmtxmatrix3.c
 * @brief 2D Matrix (3x3) math
 */

/**
 * @brief  XXX
 * @param  m0
 * @param  m1
 * @return void
 */
extern void
dmtxMatrix3Copy(DmtxMatrix3 m0, DmtxMatrix3 m1)
{
   memcpy(m0, m1, sizeof(DmtxMatrix3));
}

/**
 * @brief  XXX
 * @param  m
 * @return void
 *
 * Create Identity Transformation
 *
 *      | 1  0  0 |
 *  m = | 0  1  0 |
 *      | 0  0  1 |
 *
 *                  Transform "m"
 *            (doesn't change anything)
 *                       |\
 *  (0,1)  x----o     +--+ \    (0,1)  x----o
 *         |    |     |     \          |    |
 *         |    |     |     /          |    |
 *         +----*     +--+ /           +----*
 *  (0,0)     (1,0)      |/     (0,0)     (1,0)
 *
 */
extern void
dmtxMatrix3Identity(DmtxMatrix3 m)
{
   static DmtxMatrix3 tmp = { {1, 0, 0},
                              {0, 1, 0},
                              {0, 0, 1} };
   dmtxMatrix3Copy(m, tmp);
}

/**
 * @brief  XXX
 * @param  m
 * @param  tx
 * @param  ty
 * @return void
 *
 * Translate Transformation
 *
 *      | 1  0  0 |
 *  m = | 0  1  0 |
 *      | tx ty 1 |
 *
 *                  Transform "m"
 *                      _____    (tx,1+ty)  x----o  (1+tx,1+ty)
 *                      \   |               |    |
 *  (0,1)  x----o       /   |      (0,1)  +-|--+ |
 *         |    |      /  /\|             | +----*  (1+tx,ty)
 *         |    |      \ /                |    |
 *         +----*       `                 +----+
 *  (0,0)     (1,0)                (0,0)     (1,0)
 *
 */
void dmtxMatrix3Translate(DmtxMatrix3 m, double tx, double ty)
{
   dmtxMatrix3Identity(m);
   m[2][0] = tx;
   m[2][1] = ty;
}

/**
 * @brief  XXX
 * @param  m
 * @param  angle
 * @return void
 *
 * Rotate Transformation
 *
 *     |  cos(a)  sin(a)  0 |
 * m = | -sin(a)  cos(a)  0 |
 *     |  0       0       1 |
 *                                       o
 *                  Transform "m"      /   `
 *                       ___         /       `
 *  (0,1)  x----o      |/   \       x          *  (cos(a),sin(a))
 *         |    |      '--   |       `        /
 *         |    |        ___/          `    /  a
 *         +----*                        `+  - - - - - -
 *  (0,0)     (1,0)                     (0,0)
 *
 */
extern void
dmtxMatrix3Rotate(DmtxMatrix3 m, double angle)
{
   double sinAngle, cosAngle;

   sinAngle = sin(angle);
   cosAngle = cos(angle);

   dmtxMatrix3Identity(m);
   m[0][0] = cosAngle;
   m[0][1] = sinAngle;
   m[1][0] = -sinAngle;
   m[1][1] = cosAngle;
}

/**
 * @brief  XXX
 * @param  m
 * @param  sx
 * @param  sy
 * @return void
 *
 * Scale Transformation
 *
 *     | sx 0  0 |
 * m = | 0  sy 0 |
 *     | 0  0  1 |
 *
 *                  Transform "m"
 *                      _____     (0,sy)  x-------o (sx,sy)
 *                      \   |             |       |
 *  (0,1)  x----o       /   |      (0,1)  +----+  |
 *         |    |      /  /\|             |    |  |
 *         |    |      \ /                |    |  |
 *         +----*       `                 +----+--*
 *  (0,0)     (1,0)                (0,0)            (sx,0)
 *
 */
extern void
dmtxMatrix3Scale(DmtxMatrix3 m, double sx, double sy)
{
   dmtxMatrix3Identity(m);
   m[0][0] = sx;
   m[1][1] = sy;
}

/**
 * @brief  XXX
 * @param  m
 * @param  shx
 * @param  shy
 * @return void
 *
 * Shear Transformation
 *
 *     | 0    shy  0 |
 * m = | shx  0    0 |
 *     | 0    0    1 |
 */
extern void
dmtxMatrix3Shear(DmtxMatrix3 m, double shx, double shy)
{
   dmtxMatrix3Identity(m);
   m[1][0] = shx;
   m[0][1] = shy;
}

/**
 * @brief  XXX
 * @param  v
 * @param  m
 * @return DMTX_SUCCESS | DMTX_FAILURE
 */
extern int
dmtxMatrix3VMultiplyBy(DmtxVector2 *v, DmtxMatrix3 m)
{
   int success;
   DmtxVector2 vOut;

   success = dmtxMatrix3VMultiply(&vOut, v, m);
   *v = vOut;

   return success;
}

/**
 * @brief  XXX
 * @param  vOut
 * @param  vIn
 * @param  m
 * @return DMTX_SUCCESS | DMTX_FAILURE
 */
extern int
dmtxMatrix3VMultiply(DmtxVector2 *vOut, DmtxVector2 *vIn, DmtxMatrix3 m)
{
   double w;

   vOut->X = vIn->X*m[0][0] + vIn->Y*m[1][0] + m[2][0];
   vOut->Y = vIn->X*m[0][1] + vIn->Y*m[1][1] + m[2][1];
   w = vIn->X*m[0][2] + vIn->Y*m[1][2] + m[2][2];

   if(fabs(w) < DMTX_ALMOST_ZERO) {
      vOut->X = FLT_MAX;
      vOut->Y = FLT_MAX;
      return DMTX_FAILURE;
   }

   dmtxVector2ScaleBy(vOut, 1/w);

   return DMTX_SUCCESS;
}

/**
 * @brief  XXX
 * @param  mOut
 * @param  m0
 * @param  m1
 * @return void
 */
extern void
dmtxMatrix3Multiply(DmtxMatrix3 mOut, DmtxMatrix3 m0, DmtxMatrix3 m1)
{
   int i, j, k;
   double val;

   for(i = 0; i < 3; i++) {
      for(j = 0; j < 3; j++) {
         val = 0.0;
         for(k = 0; k < 3; k++) {
            val += m0[i][k] * m1[k][j];
         }
         mOut[i][j] = val;
      }
   }
}

/**
 * @brief  XXX
 * @param  m0
 * @param  m1
 * @return void
 */
extern void
dmtxMatrix3MultiplyBy(DmtxMatrix3 m0, DmtxMatrix3 m1)
{
   DmtxMatrix3 mTmp;

   dmtxMatrix3Copy(mTmp, m0);
   dmtxMatrix3Multiply(m0, mTmp, m1);
}

/**
 * @brief  Line skew transformation for removing perspective
 * @param  m
 * @param  b0
 * @param  b1
 * @param  sz
 * @return void
 *
 *     | b1/b0    0    (b1-b0)/(sz*b0) |
 * m = |   0    sz/b0         0        |
 *     |   0      0           1        |
 *
 *     (sz,b1)  o
 *             /|    Transform "m"
 *            / |
 *           /  |        +--+
 *          /   |        |  |
 * (0,b0)  x    |        |  |
 *         |    |      +-+  +-+
 * (0,sz)  +----+       \    /    (0,sz)  x----o
 *         |    |        \  /             |    |
 *         |    |         \/              |    |
 *         +----+                         +----+
 *  (0,0)    (sz,0)                (0,0)    (sz,0)
 *
 */
extern void
dmtxMatrix3LineSkewTop(DmtxMatrix3 m, double b0, double b1, double sz)
{
   assert(b0 > DMTX_ALMOST_ZERO);

   dmtxMatrix3Identity(m);
   m[0][0] = b1/b0;
   m[1][1] = sz/b0;
   m[0][2] = (b1 - b0)/(sz*b0);
}

/**
 * @brief  XXX
 * @param  m
 * @param  b0
 * @param  b1
 * @param  sz
 * @return void
 */
extern void
dmtxMatrix3LineSkewTopInv(DmtxMatrix3 m, double b0, double b1, double sz)
{
   assert(b1 > DMTX_ALMOST_ZERO);

   dmtxMatrix3Identity(m);
   m[0][0] = b0/b1;
   m[1][1] = b0/sz;
   m[0][2] = (b0 - b1)/(sz*b1);
}

/**
 * @brief  XXX
 * @param  m
 * @param  b0
 * @param  b1
 * @param  sz
 * @return void
 */
extern void
dmtxMatrix3LineSkewSide(DmtxMatrix3 m, double b0, double b1, double sz)
{
   assert(b0 > DMTX_ALMOST_ZERO);

   dmtxMatrix3Identity(m);
   m[0][0] = sz/b0;
   m[1][1] = b1/b0;
   m[1][2] = (b1 - b0)/(sz*b0);
}

/**
 * @brief  XXX
 * @param  m
 * @param  b0
 * @param  b1
 * @param  sz
 * @return void
 */
extern void
dmtxMatrix3LineSkewSideInv(DmtxMatrix3 m, double b0, double b1, double sz)
{
   assert(b1 > DMTX_ALMOST_ZERO);

   dmtxMatrix3Identity(m);
   m[0][0] = b0/sz;
   m[1][1] = b0/b1;
   m[1][2] = (b0 - b1)/(sz*b1);
}

/**
 * @brief  XXX
 * @param  m
 * @return void
 */
extern void
dmtxMatrix3Print(DmtxMatrix3 m)
{
   fprintf(stdout, "%8.8f\t%8.8f\t%8.8f\n", m[0][0], m[0][1], m[0][2]);
   fprintf(stdout, "%8.8f\t%8.8f\t%8.8f\n", m[1][0], m[1][1], m[1][2]);
   fprintf(stdout, "%8.8f\t%8.8f\t%8.8f\n", m[2][0], m[2][1], m[2][2]);
   fprintf(stdout, "\n");
}

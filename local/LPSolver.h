/* 
 * File:   LPSolver.h
 * Author: sriram
 *
 * Created on 5 April, 2014, 12:31 PM
 */

#ifndef LPSOLVER_H
#define	LPSOLVER_H

class LPSolver{
   enum SolverType
   {
      /// Entering Simplex.
      /** The Simplex loop for the entering Simplex can be sketched
       *  as follows:
       *  - \em Pricing : Select a variable to #ENTER the basis.
       *  - \em Ratio-Test : Select variable to #LEAVE the
       *    basis such that the basis remains feasible.
       *  - Perform the basis update.
       */
      ENTER = -1,
      /// Leaving Simplex.
      /** The Simplex loop for the leaving Simplex can be sketched
       *  as follows:
       *  - \em Pricing: Select a variable to #LEAVE the basis.
       *  - \em Ratio-Test: Select variable to #ENTER the
       *    basis such that the basis remains priced.
       *  - Perform the basis update.
       */
      LEAVE = 1
   };
};


#endif	/* LPSOLVER_H */


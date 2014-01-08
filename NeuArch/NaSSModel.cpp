/* NaSSModel.cpp */
//---------------------------------------------------------------------------

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "NaSSModel.h"


//---------------------------------------------------------------------------
// Registrar for the NaStateSpaceModel
NaConfigPart*
NaStateSpaceModel::NaRegStateSpaceModel ()
{
    return new NaStateSpaceModel();
}


//---------------------------------------------------------------------------
// Make empty (A=[0], B=[0], C=[0], D=[0]) state-space model of
// with dimension 1 for all signals.
NaStateSpaceModel::NaStateSpaceModel ()
    : A(1, 1), B(1, 1), C(1, 1), D(1, 1), x(1), x0(0),
      n(1), m(1), k(1),
      NaConfigPart(NaTYPE_StateSpaceModel)
{
    A.fetch(0,0) = 0.0;
    B.fetch(0,0) = 0.0;
    C.fetch(0,0) = 0.0;
    D.fetch(0,0) = 0.0;
}


//---------------------------------------------------------------------------
// Make an explicit copy of the state-space model
NaStateSpaceModel::NaStateSpaceModel (const NaStateSpaceModel& orig)
    : A(orig.A), B(orig.B), C(orig.C), D(orig.D), x(orig.x), x0(orig.x0),
      n(orig.n), m(orig.m), k(orig.k),
      NaConfigPart(orig)
{
}


//---------------------------------------------------------------------------
// Destructor
NaStateSpaceModel::~NaStateSpaceModel ()
{
}


//---------------------------------------------------------------------------
// Assignment
NaStateSpaceModel&
NaStateSpaceModel::operator= (const NaStateSpaceModel& orig)
{
    orig.GetMatrices(A, B, C, D);
    orig.GetDimensions(k, m, n);
}


//---------------------------------------------------------------------------
// Print self
void
NaStateSpaceModel::PrintLog () const
{
}


//---------------------------------------------------------------------------
// Print self with indentation
void
NaStateSpaceModel::PrintLog (const char* szIndent) const
{
}


//---------------------------------------------------------------------------
// Is empty (check for special condition of default constructor)
bool
NaStateSpaceModel::Empty () const
{
    return (1 == A.dim_rows() && 1 == A.dim_cols() && 0.0 == A.get(0, 0) &&
	    1 == B.dim_rows() && 1 == B.dim_cols() && 0.0 == B.get(0, 0) &&
	    1 == C.dim_rows() && 1 == C.dim_cols() && 0.0 == C.get(0, 0) &&
	    1 == D.dim_rows() && 1 == D.dim_cols() && 0.0 == D.get(0, 0));
}


//---------------------------------------------------------------------------
// Make empty
void
NaStateSpaceModel::Clean ()
{
    A.new_dim(1, 1);
    B.new_dim(1, 1);
    C.new_dim(1, 1);
    D.new_dim(1, 1);

    n = m = k = 1;

    A.fetch(0,0) = 0.0;
    B.fetch(0,0) = 0.0;
    C.fetch(0,0) = 0.0;
    D.fetch(0,0) = 0.0;
}


//***********************************************************************
// Custom part
//***********************************************************************

//---------------------------------------------------------------------------
// Set initial state.
void
NaStateSpaceModel::SetInitialState (const NaVector& vX0)
{
    x0 = vX0;
}


//---------------------------------------------------------------------------
// Set zero initial state (zero vector).
void
NaStateSpaceModel::SetZeroInitialState ()
{
    x0.new_dim(0);
}


//---------------------------------------------------------------------------
// Set space-state model with matrices A, B, C, D, where:
//  { x(t+1) = A*x(t) + B*u(t)
//  { y(t)   = C*x(t) + D*u(t)
void
NaStateSpaceModel::SetMatrices (const NaMatrix& mA,
				const NaMatrix& mB,
				const NaMatrix& mC,
				const NaMatrix& mD)
{
    if(!(mA.dim_cols() > 0 && mA.dim_rows() > 0 &&
         mB.dim_cols() > 0 && mB.dim_rows() > 0 &&
         mC.dim_cols() > 0 && mC.dim_rows() > 0 &&
         mD.dim_cols() > 0 && mD.dim_rows() > 0 &&
         mA.dim_rows() == mA.dim_cols() &&
         mA.dim_rows() == mB.dim_rows() &&
         mA.dim_rows() == mC.dim_cols() &&
         mC.dim_rows() == mD.dim_rows() &&
         mB.dim_cols() == mD.dim_cols())){
	throw(na_size_mismatch);
    }

    A = mA;
    B = mB;
    C = mC;
    D = mD;

    n = A.dim_rows();
    m = C.dim_rows();
    k = B.dim_cols();
}


//---------------------------------------------------------------------------
// Shorter form to set space-state model matrices A, B, C,
// where:
//  { x(t+1) = A*x(t) + B*u(t)
//  { y(t)   = C*x(t)
void
NaStateSpaceModel::SetMatrices (const NaMatrix& mA,
				const NaMatrix& mB,
				const NaMatrix& mC)
{
    if(!(mA.dim_cols() > 0 && mA.dim_rows() > 0 &&
         mB.dim_cols() > 0 && mB.dim_rows() > 0 &&
         mC.dim_cols() > 0 && mC.dim_rows() > 0 &&
         mA.dim_rows() == mA.dim_cols() &&
         mA.dim_rows() == mB.dim_rows() &&
         mA.dim_rows() == mC.dim_cols())){
         throw(na_size_mismatch);
    }

    A = mA;
    B = mB;
    C = mC;
    D.new_dim(C.dim_rows(), B.dim_cols());
    D.init_zero();

    n = A.dim_rows();
    m = C.dim_rows();
    k = B.dim_cols();
}


//---------------------------------------------------------------------------
// The shortest form to set space-state model matrices A, B,
// where:
//  { x(t+1) = A*x(t) + B*u(t)
//  { y(t)   = x(t)
void
NaStateSpaceModel::SetMatrices (const NaMatrix& mA,
				const NaMatrix& mB)
{
    if(!(mA.dim_cols() > 0 && mA.dim_rows() > 0 &&
         mB.dim_cols() > 0 && mB.dim_rows() > 0 &&
         mA.dim_rows() == mA.dim_cols() &&
         mA.dim_rows() == mB.dim_rows())){
         throw(na_size_mismatch);
    }

    A = mA;
    B = mB;
    C.new_dim(A.dim_rows(), A.dim_cols());
    C.init_diag(1.0);
    D.new_dim(C.dim_rows(), B.dim_cols());
    D.init_zero();

    n = A.dim_rows();
    m = C.dim_rows();
    k = B.dim_cols();
}


//---------------------------------------------------------------------------
// Get matrices.
void
NaStateSpaceModel::GetMatrices (NaMatrix& mA,
				NaMatrix& mB,
				NaMatrix& mC,
				NaMatrix& mD) const
{
    mA = A;
    mB = B;
    mC = C;
    mD = D;
}


//---------------------------------------------------------------------------
// Get dimensions.
void
NaStateSpaceModel::GetDimensions (unsigned& uInputs,
				  unsigned& uOutputs,
				  unsigned& uState) const
{
    uInputs = k;
    uOutputs = m;
    uState = n;
}


//***********************************************************************
// Unit part
//***********************************************************************

//---------------------------------------------------------------------------
// Reset operations, that must be done before new modelling
// will start
void
NaStateSpaceModel::Reset ()
{
    if(x0.dim() == 0)
	x.init_zero();
    else if(x0.dim() == x.dim())
	x.copy(x0);
    else
	throw(na_size_mismatch);
}


//---------------------------------------------------------------------------
// Compute output on the basis of internal parameters,
// stored state and external input: y=F(u,t,p)
void
NaStateSpaceModel::Function (NaReal* pU, NaReal* pY)
{
    NaVector	u(k), y(m);
    u = pU;

    NaVector    x1(n), x2(n), y1(m);

    // y(t) = C*x(t) + D*u(t)
    C.multiply(x, y);            // y = C * x
    D.multiply(u, y1);           // y1 = D * u
    y.add(y1);                   // y += y1

    // x(t+1) = A*x(t) + B*u(t)
    A.multiply(x, x1);           // x1 = A * x
    B.multiply(u, x2);           // x2 = B * u
    x.copy(x1);                  // x = x1
    x.add(x2);                   // x += x2

    if(pY != NULL) {
	for(unsigned i = 0; i < y.dim(); ++i)
	    pY[i] = y[i];
    }
}


//***********************************************************************
// Store and retrieve configuration data
//***********************************************************************

//---------------------------------------------------------------------------
// Store configuration data in internal order to given stream
void
NaStateSpaceModel::Save (NaDataStream& ds)
{
}


//---------------------------------------------------------------------------
// Retrieve configuration data in internal order from given stream
void
NaStateSpaceModel::Load (NaDataStream& ds)
{
}


//---------------------------------------------------------------------------
// Store state-space model to given configuration file
void
NaStateSpaceModel::Save (const char* szFileName)
{
}


//---------------------------------------------------------------------------
// Read state-space model from given configuration file
void
NaStateSpaceModel::Load (const char* szFileName)
{
}

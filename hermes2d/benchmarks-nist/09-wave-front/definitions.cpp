#include "weakform/weakform.h"
#include "integrals/integrals_h1.h"
#include "boundaryconditions/essential_bcs.h"
#include "weakform_library/laplace.h"

/* Right-hand side */

class CustomRightHandSide: public DefaultNonConstRightHandSide
{
public:
  CustomRightHandSide(double alpha, double x_loc, double y_loc, double r_zero) 
    : DefaultNonConstRightHandSide(alpha, x_loc, y_loc, r_zero) { };

  virtual double value(double x, double y) {  
    double a = pow(x - coeff2, 2);
    double b = pow(y - coeff3, 2);
    double c = sqrt(a + b);
    double d = ((coeff1*x - (coeff1 * coeff2)) * (2*x - (2 * coeff2)));
    double e = ((coeff1*y - (coeff1 * coeff3)) * (2*y - (2 * coeff3)));
    double f = (pow(coeff1*c - (coeff1 * coeff4), 2) + 1.0);
    double g = (coeff1 * c - (coeff1 * coeff4));

    return -(((coeff1/(c * f)) - (d/(2 * pow(a + b, 1.5) * f)) 
           - ((coeff1 * d * g)/((a + b) * pow(f, 2))) + 
	      (coeff1/(c * f)) - (e/(2 * pow(a + b, 1.5) * f)) 
           - ((coeff1 * e * g)/((a + b) * pow(f, 2)))));
  }

  virtual Ord ord (Ord x, Ord y) {
    return Ord(8);  
  }
};


/* Exact solution */

class CustomExactSolution : public ExactSolutionScalar
{
public:
  CustomExactSolution(Mesh* mesh, double alpha, double x_loc, double 
                      y_loc, double r_zero) 
             : ExactSolutionScalar(mesh), alpha(alpha), x_loc(x_loc), 
                                   y_loc(y_loc), r_zero(r_zero) { }

  virtual scalar value(double x, double y) {
    return atan(alpha * (sqrt(pow(x - x_loc, 2) + pow(y - y_loc, 2)) - r_zero));
  };

  virtual void derivatives (double x, double y, scalar& dx, scalar& dy) {
    double a = pow(x - x_loc, 2);
    double b = pow(y - y_loc, 2);
    double c = sqrt(a + b);
    double d = (alpha*x - (alpha * x_loc));
    double e = (alpha*y - (alpha * y_loc));
    double f = (pow(alpha*c - (alpha * r_zero), 2) + 1.0);

    dx = (d/(c * f));
    dy = (e/(c * f));
  };

  Ord ord (Ord x, Ord y) {
    return Ord(8);  
  }

  double alpha, x_loc, y_loc, r_zero;
};

/* Weak forms */

class CustomWeakFormPoisson : public WeakForm
{
public:
  CustomWeakFormPoisson(DefaultNonConstRightHandSide* rhs) : WeakForm(1) {
    add_matrix_form(new DefaultMatrixFormVolConst(0, 0));
    add_vector_form(new DefaultVectorFormVolNonConst(0, rhs));
  };
};


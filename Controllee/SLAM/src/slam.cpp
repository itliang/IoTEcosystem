#include <iostream>
#include <Eigen/Core>

using namespace std;


int main(int argc, char **argv)
{
	Eigen::Matrix<float, 2, 3> matrix_23;
	Eigen::Vector3d v_3d;
	//Eigen::Matrix3d matrix_33 = Eigen::Matrix::zero();
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix_dynamic;
	Eigen::MatrixXd matrix_x;

	matrix_23<< 1, 2, 3, 4, 5, 6;
	cout << matrix_23 << endl;

	for(int i=0; i< 1; i++){
		for(int j=0; j< 2; j++){
			cout << matrix_23(i, j) << endl;
		}
	}

	v_3d << 3, 4, 5;
	
	
}



#include "sparse_matrix.hh"

int SparseMatrix::get(int r, int c) const
{
	// Retorna el elemento que hay en la posicion (r,c)
	//busca donde empieza y donde termina la fila
	int opening = rowPtr[r];
	int ending = rowPtr[r+1];
	int result = 0;
	//recorre las columnas de la fila buscando la solicitada
	for (int i = opening; i < ending; i++) {
		if (colInd[i] == c) {
			//si la encuentra deja de recorrer
			result = val[i];
			break;
		}
	}
	// si recorrio toda la fila y no encontro la columna, result es 0
	// si se escribio algun valor en result, lo retorna
	return result;
}

void SparseMatrix::set(int v, int r, int c)
{
	//redefinicion del set
	//se asume que la fuente esta ordenada
	val.push_back( v );
	colInd.push_back( c );
	rowPtr[r+1]=counter+1;
	counter++;
	return;
}

SparseMatrix SparseMatrix::mult(const SparseMatrix& b)
{
	// Multiplica esta matriz con la matriz b
	// se crea la matriz resultado, de filas como la propia matriz t de columnas como la segunda
	int r = rows;
	int c = b.cols;
	SparseMatrix result(r,c);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			// aca se comienza, para cada posicion i,j de la matriz resultado
			// el calculo de la multiplicacion de matrices.
			int opening = rowPtr[i];
			int ending = rowPtr[i+1];
			int value = 0;
			for (int k = opening; k < ending; k++) {
				//se recorren las columnas con valores
				int tc = colInd[k];
				int v1 = val[k];
				// aca me estaba equivocando en el orden fila/columna
				// que para b se debe tomar columna/fila, ya que se recorre al contrario :P
				int v2 = b.get(tc,j);
				value += v1*v2;
			}
			if (value != 0) {
				result.set(value,i,j);
			}
			//fin del proceso de multiplicacion
		}
	}
	return result;
}

void SparseMatrix::mult_row(int a,Vector& rv,Vector& rc)
{
	int x1 = rowPtr[a];
	int y1 = rowPtr[a+1];
	//se recorren las columnas correspondientes
	//a las filas que tienen valores
	//
	for (int b = 0; b < rows; ++b)
	{
		//para cada columna
		int x2 = rowPtr[b];
		int y2 = rowPtr[b+1];
		int r=INF; //valor
		for (int i = x1; i < y1; ++i)
		{
			//se recorre cada elemento de la columna
			for (int j = x2; colInd[i] >= colInd[j] and j < y2; ++j)
			{
				//busca si existe un valor en la fila correspondiente
				if (colInd[i] == colInd[j])
				{
					//aqui se efectua la multiplicacion
					//r += val[i]*val[j];
					r = min(r,val[i] + val[j]);
				}
			}
		}
		if (r != 0)
		{
			//si luego de calcular, hay algun valor
			rv.push_back(r);
			rc.push_back(b);
		}
	}
	return;
}

	void SparseMatrix::print()
	{
		// imprime la matriz dispersa como los 3 vectores
		//se imprimen los valores
		cout << "Values = ";
		for (const auto i: val)
			cout << i << '\t';
		cout << endl;
		//se imprimen las columnas
		cout << "ColInd = ";
		for (const auto i: colInd)
			cout << i << '\t';
		cout << endl;
		//se imprimen las filas
		cout << "RowPtr = ";
		for (const auto i: rowPtr)
			cout << i << '\t';
		cout << endl;
	}
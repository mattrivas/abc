/*
class Matrix represents a matrix
operator[](int i) returns a reference to the ith Row
Row is an internal type that simply defines the operator[](int j) to return
the ith element in a Row (which is a T&)
*/
template <typename T>
class Matrix {
    int col, row;
    typedef std::vector<T> Row;

    std::vector<Row> data;

public:
    Matrix(int c, int r): row(r), col(c), data(c, std::vector<T>(r)) {}
    Row & operator[](int i) {
        return data[i];
    }
}
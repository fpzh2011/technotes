/* 163.html */ 

class SquareDigits {
public:
	unsigned smallestResult (unsigned n);
private:
	unsigned sod (unsigned i); //返回i的各位数字的平方和
	bool contain (unsigned n, unsigned t); //T(t)是否包含n
};

#include "lingeo3D.h"
#include <cmath>
#include <iostream>



using namespace lingeo3D;

const float pi_ = 3.141593;
float lingeo3D::inter_area_width = 100.0;


//*********STRUCT point_t BEGIN**********

template<typename T>
point_t<T>::point_t(T x, T y, T z): x_(x), y_(y), z_(z){

}
template<typename T>
point_t<T>::point_t(const point_t<T> &point): x_(point.x_), y_(point.y_), z_(point.z_){

}


template<typename T>
void point_t<T>::print() const{
	std::cout << x_ << " " << y_ << " " << z_ << std::endl;
}

template<typename T>
T point_t<T>::distance(point_t<T> const &pnt) const{
	T dx = x_ - pnt.x_;
	T dy = y_ - pnt.y_;
	T dz = z_ - pnt.z_;

	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

template<typename T>
T point_t<T>::scalar_prod(point_t<T> const &pnt) const{
	return x_*pnt.x_ + y_*pnt.y_ + z_*pnt.z_;
}

template<typename T>
point_t<T> point_t<T>::vector_prod(point_t<T> const &pnt) const{
	return point_t<T>{y_ * pnt.z_ - pnt.y_ * z_, -x_ * pnt.z_ + pnt.x_ * z_, x_ * pnt.y_ - pnt.x_ * y_};
}

template<typename T>
bool point_t<T>::valid() const{
	return (x_ != x_ || y_ != y_ || z_ != z_) ? false : true;
}


//*********STRUCT point_t END************

//*********STRUCT line_t BEGIN***********

template<typename T>
line_t<T>::line_t(): root_{0.0, 0.0, 0.0}, dir_{1.0, 1.0, 1.0}{

}

template<typename T>
line_t<T>::line_t(struct point_t<T> a1, struct point_t<T> a2): root_{a1}, dir_{a2 - a1}{

}


template<typename T>
bool line_t<T>::perpendicular(line_t<T> const &another) const{
	T scalar_product = dir_.x_ * another.dir_.x_ + dir_.y_ * another.dir_.y_ + dir_.z_ * another.dir_.z_;
	return (std::abs(scalar_product) < flt_tolerance) ? true : false;
}


template<typename T>
bool line_t<T>::valid() const{
	return root_.valid() && dir_.valid() && dir_ != point_t<T>{0.0, 0.0};
}
template<typename T>
void line_t<T>::print() const{
	std::cout << "Root is: "; root_.print();
	std::cout << "; Dir is: "; dir_.print();
	std::cout << std::endl;
}

template<typename T>
T line_t<T>::angle(point_t<T> const &pnt1, point_t<T> const &pnt2) const{
	plane_t<T> pln1{*this, pnt1};
	plane_t<T> pln2{*this, pnt2};
	if(!pln1.valid() || !pln2.valid())
		return NAN;
	point_t<T> norm1 = pln1.get_normal();
	point_t<T> norm2 = pln2.get_normal();
	point_t<T> vect_prod = norm1.vector_prod(norm2);
	int sign = ((((vect_prod.x_ > 0) == (dir_.x_ > 0)) && std::abs(dir_.x_) > flt_tolerance) ||
				(((vect_prod.y_ > 0) == (dir_.y_ > 0)) && std::abs(dir_.y_) > flt_tolerance) ||
				(((vect_prod.z_ > 0) == (dir_.z_ > 0)) && std::abs(dir_.z_) > flt_tolerance)   ) ? 1 : -1;

	T mod1 = norm1.distance({0.0, 0.0, 0.0});
	T mod2 = norm2.distance({0.0, 0.0, 0.0});
	T scalar = norm1.scalar_prod(norm2);
	T angle = std::acos(scalar/(mod1 * mod2));
	if(sign < 0)
		angle = 2 * pi_ - angle;
	return angle;
}


//*********STRUCT line_t END*************

//*********STRUCT plane_t BEGIN**********

template<typename T>
plane_t<T>::plane_t(T a, T b, T c, T d): a_(a), b_(b), c_(c), d_(d){

}
template<typename T>
plane_t<T>::plane_t(point_t<T> p1, point_t<T> p2, point_t<T> p3){
	point_t<T> v1 = p2 - p1;
	point_t<T> v2 = p3 - p1;
	a_ = v1.y_ * v2.z_ - v2.y_ * v1.z_;
	b_ = -(v1.x_ * v2.z_ - v2.x_ * v1.z_);
	c_ = v1.x_ * v2.y_ - v2.x_ * v1.y_;
	d_ =-(a_ * p1.x_ + b_ * p1.y_ + c_ * p1.z_);
}
template<typename T>
plane_t<T>::plane_t(line_t<T> line, point_t<T> pnt): plane_t<T>(pnt, line.get_root(), line.get_dir() + line.get_root()){

}


template<typename T>
point_t<T> plane_t<T>::intersection(line_t<T> const &line) const{
	line_t<T> normal(point_t<T>{0.0, 0.0, 0.0}, point_t<T>{a_, b_, c_});
	if(line.perpendicular(normal)){
		return point_t<T>{};
	}

	point_t<T> root = line.get_root();
	point_t<T> dir = line.get_dir();

	T param = -(root.x_ * a_ + root.y_ * b_ + root.z_ * c_ + d_)/(dir.x_ * a_ + dir.y_ * b_ + dir.z_ * c_);

	point_t<T> ret(root + dir * param);
	return ret;

}

template<typename T>
point_t<T> plane_t<T>::get_normal() const{
	return {a_, b_, c_};
}

template<typename T>
enum areas_t plane_t<T>::get_side_area(point_t<T> const &point) const{
	T side_offset = a_ * point.x_ + b_ * point.y_ + c_ * point.z_ + d_;

	if(side_offset > 0.0 + flt_tolerance * inter_area_width)
		return LEFT_SIDE;
	
	if(side_offset > 0.0 - flt_tolerance * inter_area_width)
		return INTER_SIDE;
	
	return 
	RIGHT_SIDE;

}


template<typename T>
bool plane_t<T>::separates(point_t<T> const &pnt1, point_t<T> const &pnt2) const{
	enum areas_t side1 = get_side_area(pnt1);
	enum areas_t side2 = get_side_area(pnt2);
	if(side1 == INTER_SIDE || side2 == INTER_SIDE)
		return false;
	return (side1 == side2) ? false : true;
}

template<typename T>
bool plane_t<T>::valid() const{
	point_t<T> norm = get_normal();
	return (!norm.valid() || d_ != d_) ? false : true;
}

template<typename T>
void plane_t<T>::print() const{
	const char* str_1 = (b_ > 0.0) ? "+ " : "- ";
	const char* str_2 = (c_ > 0.0) ? "+ " : "- ";
	const char* str_3 = (d_ > 0.0) ? "+ " : "- ";

	std::cout << a_ << "x " << str_1 << std::abs(b_) << "y " << str_2 << std::abs(c_) << "z " << str_3 << std::abs(d_) << " = 0" << std::endl;

}


//*********STRUCT plane_t END************

//*********STRUCT polygon_t BEGIN********

template<typename T>
lingeo3D::polygon_t<T>::polygon_t(){

}

template<typename T>
polygon_t<T>::polygon_t(std::vector<point_t<T>> vertices){
	this -> vertices = vertices;

}     // constructing by vector<point_t> array

template<typename T>
T polygon_t<T>::square() const{
	//TODO
	return 0.0;
}

template<typename T>
void polygon_t<T>::print() const{
	for(int i = 0; i < vertices.size(); i++)
		vertices[i].print();
	std::cout << std::endl;
}

template<typename T>
line_t<T> polygon_t<T>::get_side(int index) const{
	while(index < 0)
		index += vertices.size();

	line_t<T> ret(vertices[index % vertices.size()], vertices[(index + 1) % vertices.size()]);
	
	return ret;
}

template<typename T>
bool polygon_t<T>::valid() const{
	if(vertices.size() < 3)
		return false;
	
	for(int i = 0; i < vertices.size(); i++)
		if(!vertices[i].valid())
			return false;

	return true;
}

template<typename T>
bool polygon_t<T>::is_divided_by_side_plane(polygon_t<T> const & another) const{
	for(int i = 0; i < vertices.size(); i++){
		line_t<T> line = get_side(i);
		point_t<T> pnt = vertices[(i + 2) % vertices.size()];
		T max_angle = 0.0;
		T min_angle = pi_ * 2.0;
		for(int j = 0; j < another.vertices.size(); j++){
			T angle = line.angle(pnt, another.vertices[j]);
			if(angle != angle)
				return false;
			if(angle < min_angle) min_angle = angle;
			if(angle > max_angle) max_angle = angle;
		}
		if(max_angle - min_angle < pi_ && min_angle != 0.0 && max_angle != pi_* 2){
			return true;
		}
	}
	return false;	
}

template<typename T>
bool polygon_t<T>::intersect(polygon_t<T> const & another) const{
	if(!valid() || !another.valid()){
		return false;
	}

	return (is_divided_by_side_plane(another) || another.is_divided_by_side_plane(*this)) ? false : true;

}

template<typename T>
bool polygon_t<T>::holding(point_t<T> const & vert) const{
	for(int i = 0; i < vertices.size(); i++)
		if(vert == vertices[i])
			return true;
	return false;	
}


template<typename T>
void polygon_t<T>::add(point_t<T> const &vert){
	vertices.insert(vertices.end(), vert);
}

//*********STRUCT polygon_t END**********

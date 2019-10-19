#include "lingeo3D.h"
#include <cmath>
#include <iostream>



using namespace lingeo3D;

const float pi_ = 3.141593;
const float lingeo3D::flt_tolerance = 0.00001;
float lingeo3D::inter_area_width = 100.0;


//*********STRUCT point_t BEGIN**********

point_t::point_t(float x, float y, float z): x_(x), y_(y), z_(z){

}
point_t::point_t(const point_t &point): x_(point.x_), y_(point.y_), z_(point.z_){

}


void point_t::print() const{
	std::cout << "(" << x_ << ", " << y_ << ", " << z_ << ")" << std::endl;
}

float point_t::distance(point_t const &pnt) const{
	float dx = x_ - pnt.x_;
	float dy = y_ - pnt.y_;
	float dz = z_ - pnt.z_;

	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float point_t::scalar_prod(point_t const &pnt) const{
	return x_*pnt.x_ + y_*pnt.y_ + z_*pnt.z_;
}

point_t point_t::vector_prod(point_t const &pnt) const{
	return point_t{y_ * pnt.z_ - pnt.y_ * z_, -x_ * pnt.z_ + pnt.x_ * z_, x_ * pnt.y_ - pnt.x_ * y_};
}

bool point_t::valid() const{
	return (x_ != x_ || y_ != y_ || z_ != z_) ? false : true;
}
bool point_t::operator==(point_t const &pnt) const{
	return (std::abs(x_ - pnt.x_) < flt_tolerance && std::abs(y_ - pnt.y_) < flt_tolerance && std::abs(z_ - pnt.z_) < flt_tolerance) ? true : false;

}
bool point_t::operator!=(point_t const &pnt) const{
	return !operator==(pnt);
}

point_t point_t::operator-(point_t const &another) const{
	point_t ret(x_ - another.x_, y_ - another.y_, z_ - another.z_);
	return ret;
}

point_t point_t::operator+(point_t const &another) const{
	point_t ret(x_ + another.x_, y_ + another.y_, z_ + another.z_);
	return ret;	
}
point_t point_t::operator*(float num) const{
	point_t ret(x_ * num, y_ * num, z_ * num);
	return ret;	
}


float point_t::get_x() const{
	return x_;
}
float point_t::get_y() const{
	return y_;
}
float point_t::get_z() const{
	return z_;
}


//*********STRUCT point_t END************

//*********STRUCT line_t BEGIN***********

line_t::line_t(): root_{0.0, 0.0, 0.0}, dir_{1.0, 1.0, 1.0}{

}

line_t::line_t(struct point_t a1, struct point_t a2): root_{a1}, dir_{a2 - a1}{

}


bool line_t::perpendicular(line_t const &another) const{
	float scalar_product = dir_.get_x() * another.dir_.get_x() + dir_.get_y() * another.dir_.get_y() + dir_.get_z() * another.dir_.get_z();
	return (std::abs(scalar_product) < flt_tolerance) ? true : false;
}


bool line_t::valid() const{
	return root_.valid() && dir_.valid() && dir_ != point_t{0.0, 0.0};
}
void line_t::print() const{
	std::cout << "Root is: "; root_.print();
	std::cout << "; Dir is: "; dir_.print();
	std::cout << std::endl;
}

float line_t::angle(point_t const &pnt1, point_t const &pnt2) const{
	plane_t pln1{*this, pnt1};
	plane_t pln2{*this, pnt2};
	if(!pln1.valid() || !pln2.valid())
		return NAN;
	point_t norm1 = pln1.get_normal();
	point_t norm2 = pln2.get_normal();
	point_t vect_prod = norm1.vector_prod(norm2);
	int sign = ((((vect_prod.get_x() > 0) == (dir_.get_x() > 0)) && std::abs(dir_.get_x()) > flt_tolerance) ||
				(((vect_prod.get_y() > 0) == (dir_.get_y() > 0)) && std::abs(dir_.get_y()) > flt_tolerance) ||
				(((vect_prod.get_z() > 0) == (dir_.get_z() > 0)) && std::abs(dir_.get_z()) > flt_tolerance)   ) ? 1 : -1;

	float mod1 = norm1.distance({0.0, 0.0, 0.0});
	float mod2 = norm2.distance({0.0, 0.0, 0.0});
	float scalar = norm1.scalar_prod(norm2);
	float angle = std::acos(scalar/(mod1 * mod2));
	if(sign < 0)
		angle = 2 * pi_ - angle;
	return angle;
}

point_t line_t::get_root() const{
	return root_;
}

point_t line_t::get_dir() const{
	return dir_;
}



//*********STRUCT line_t END*************

//*********STRUCT plane_t BEGIN**********

plane_t::plane_t(float a, float b, float c, float d): a_(a), b_(b), c_(c), d_(d){

}
plane_t::plane_t(point_t p1, point_t p2, point_t p3){
	point_t v1 = p2 - p1;
	point_t v2 = p3 - p1;
	a_ = v1.get_y() * v2.get_z() - v2.get_y() * v1.get_z();
	b_ = -(v1.get_x() * v2.get_z() - v2.get_x() * v1.get_z());
	c_ = v1.get_x() * v2.get_y() - v2.get_x() * v1.get_y();
	d_ =-(a_ * p1.get_x() + b_ * p1.get_y() + c_ * p1.get_z());
}
plane_t::plane_t(line_t line, point_t pnt): plane_t(pnt, line.get_root(), line.get_dir() + line.get_root()){

}


point_t plane_t::intersection(line_t const &line) const{
	line_t normal(point_t{0.0, 0.0, 0.0}, point_t{a_, b_, c_});
	if(line.perpendicular(normal)){
		return point_t{};
	}

	point_t root = line.get_root();
	point_t dir = line.get_dir();

	float param = -(root.get_x() * a_ + root.get_y() * b_ + root.get_z() * c_ + d_)/(dir.get_x() * a_ + dir.get_y() * b_ + dir.get_z() * c_);

	point_t ret(root + dir * param);
	return ret;

}

point_t plane_t::get_normal() const{
	return {a_, b_, c_};
}

enum areas_t plane_t::get_side_area(point_t const &point) const{
	float side_offset = a_ * point.get_x() + b_ * point.get_y() + c_ * point.get_z() + d_;

	if(side_offset > 0.0 + flt_tolerance * inter_area_width)
		return LEFT_SIDE;
	
	if(side_offset > 0.0 - flt_tolerance * inter_area_width)
		return INTER_SIDE;
	
	return 
	RIGHT_SIDE;

}


bool plane_t::separates(point_t const &pnt1, point_t const &pnt2) const{
	enum areas_t side1 = get_side_area(pnt1);
	enum areas_t side2 = get_side_area(pnt2);
	if(side1 == INTER_SIDE || side2 == INTER_SIDE)
		return false;
	return (side1 == side2) ? false : true;
}

bool plane_t::valid() const{
	point_t norm = get_normal();
	return (!norm.valid() || d_ != d_) ? false : true;
}

void plane_t::print() const{
	const char* str_1 = (b_ > 0.0) ? "+ " : "- ";
	const char* str_2 = (c_ > 0.0) ? "+ " : "- ";
	const char* str_3 = (d_ > 0.0) ? "+ " : "- ";

	std::cout << a_ << "x " << str_1 << std::abs(b_) << "y " << str_2 << std::abs(c_) << "z " << str_3 << std::abs(d_) << " = 0" << std::endl;

}


//*********STRUCT plane_t END************

//*********STRUCT polygon_t BEGIN********

polygon_t::polygon_t(){

}
                                  // empty polygon
polygon_t::polygon_t(std::vector<point_t> vertices){
	this -> vertices = vertices;

}     // constructing by vector<point_t> array

float polygon_t::square() const{
	//TODO
	return 0.0;
}

void polygon_t::print() const{
	for(int i = 0; i < vertices.size(); i++)
		vertices[i].print();
	std::cout << std::endl;
}

line_t polygon_t::get_side(int index) const{
	while(index < 0)
		index += vertices.size();

	line_t ret(vertices[index % vertices.size()], vertices[(index + 1) % vertices.size()]);
	
	return ret;
}

bool polygon_t::valid() const{
	if(vertices.size() < 3)
		return false;
	
	for(int i = 0; i < vertices.size(); i++)
		if(!vertices[i].valid())
			return false;

	return true;
}

bool polygon_t::is_divided_by_side_plane(polygon_t const & another) const{
	for(int i = 0; i < vertices.size(); i++){
		line_t line = get_side(i);
		point_t pnt = vertices[(i + 2) % vertices.size()];
		float max_angle = 0.0;
		float min_angle = pi_ * 2.0;
		for(int j = 0; j < another.vertices.size(); j++){
			float angle = line.angle(pnt, another.vertices[j]);
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

bool polygon_t::intersect(polygon_t const & another) const{
	if(!valid() || !another.valid()){
		return false;
	}

	return (is_divided_by_side_plane(another) || another.is_divided_by_side_plane(*this)) ? false : true;

}

bool polygon_t::holding(point_t const & vert) const{
	for(int i = 0; i < vertices.size(); i++)
		if(vert == vertices[i])
			return true;
	return false;	
}


void polygon_t::add(point_t const &vert){
	vertices.insert(vertices.end(), vert);
}

//*********STRUCT polygon_t END**********

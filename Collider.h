#pragma once

class Collider {
public:
	//���a���擾
	float GetRadius() { return radius_; }
	
	//���a��ݒ�
	void SetRadius() {}

private:
	float radius_ = 0;
};
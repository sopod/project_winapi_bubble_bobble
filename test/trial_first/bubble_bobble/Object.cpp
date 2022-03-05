#include "Object.h"

Object::Object()
{
	gravity = 1;
	jumpPower = -15.0f;
	jumpLeft = 0;
	moveSpeed = 3.0;
	IsOnGround = true;
	IsJumping = false;
}

Object::~Object()
{
}

//����: https://www.youtube.com/watch?v=8JJ-4JgR7Dg&list=LL&index=5
bool Object::RectVsRect(FPOINT & r1, int size1, FPOINT & r2, int size2)
{
	return (r1.x < r2.x + size2 && r1.x + size1 > r2.x && r1.y < r2.y + size2 && r1.y + size1 > r2.y);
}

bool Object::RayVsRect(FPOINT & ray_origin, FPOINT & ray_dir, FPOINT & target_pos, int target_size, 
	FPOINT & contact_point, FPOINT & contact_normal, float & t_hit_near)
{
	// �ʱ�ȭ
	contact_normal = { 0,0 };
	contact_point = { 0,0 };

	// ray_dir���� ������ ���� ĳ�����ش�
	FPOINT invdir = 1.0f / ray_dir; 

	// ray_origin���� ray_dir���� time�� 0 ~ 1�̶�� �������� ��,
	// Near tx, ty = target �簢���� ���� �ε��� x����, y������ ray_dir ��� �� %���� (0 ~ +-1 ������ ��) = �� ������ ���� time�� �ȴ�.
	// Far tx, ty = ���߿� �ε���
	FPOINT t_near = (target_pos - ray_origin) * invdir;
	FPOINT t_far = (target_pos + target_size - ray_origin) * invdir;

	// 0���� ������ �� = ray_dir�� 0�� �� = ���ڰ� �ƴѰ��� ���´� = isNan = true
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;
	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false; 

	// Near�� Far ���� �� �ְ� ���Դٸ� �������ش� (���ϴܿ��� �������� �������� ���)
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	// �Ʒ� ��츸 �簢���� �浹�� �Ͼ ����. 
	// Near.x < Far.y
	// Near.y < Far.x 
	// �̰��� ������ ���� false
	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

	// ���� ������ �浹�� ���� t = Near.x Near.y �߿� ū ��
	t_hit_near = max(t_near.x, t_near.y);

	// ���� �ְ� �浹�� ���� t = Far.x Far.y �߿� ���� ��
	float t_hit_far = min(t_far.x, t_far.y);

	// ���� �ְ� �浹�� ������ ���̳ʽ��� = ������ �浹�� ������ ���̳ʽ��� = ay direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// �浹 ���� = ������ġ + �̵��� * �浹 ���������� �ð� t
	contact_point = ray_origin + ray_dir * t_hit_near;

	// ��ֺ��� �˾Ƴ���
	// Near.x > Near.y �̰� ray_dir�� ����̸� x - 1
	//                                �����̸� x + 1
	// Near.x < Near.y �̰� ray_dir�� ����̸� y - 1
	//                                �����̸� y + 1 
	// Near.x = Near.y �̰� ray_dir.x ray_dir.y 
	//                           +     +    -1, -1
	//                           +     -    -1,  1
	//                           -     +     1, -1
	//                           -     -     1,  1
	if (t_near.x > t_near.y)
	{
		if (invdir.x < 0)
			contact_normal = { 1, 0 };
		else
			contact_normal = { -1, 0 };
	}
	else if (t_near.x < t_near.y)
	{
		if (invdir.y < 0)
			contact_normal = { 0, 1 };
		else
			contact_normal = { 0, -1 };

	}/*
	else if (t_near.x / t_near.y == t_far.x / t_far.y)
	{
		if (invdir.x > 0)
		{
			if (invdir.y > 0)
				contact_normal = { -1, -1 };
			else if (invdir.y < 0)
				contact_normal = { -1, 1 };
		}
		else if (invdir.x < 0)
		{
			if (invdir.y > 0)
				contact_normal = { 1, -1 };
			else if (invdir.y < 0)
				contact_normal = { 1, 1 };
		}
	}*/
		
	

	// Note if t_near == t_far, collision is principly in a diagonal
	// so pointless to resolve. By returning a CN={0,0} even though its
	// considered a hit, the resolver won't change anything.
	return true;
}

bool Object::DynamicRectVsRect(Object & in, FPOINT & target_pos, int & target_size, 
	FPOINT & contact_point, FPOINT & contact_normal, float & contact_time, float fElapsedTime)
{
	// ������ ���� ���ٸ� false
	if (in.vel.x == 0 && in.vel.y == 0)
		return false;

	// �浹 ó�� �� �÷��̾� ��ġ�� �簢�� ���̸� �ȵǴϱ� �簢�� ũ�⸦ �÷��ش�.
	// �簢�� ��ġ = ���� ��ġ - �÷��̾� ũ���� ����
	// �簢�� ũ�� = ���� ũ�� + �÷��̾� ũ��
	FPOINT expanded_target_pos = target_pos - in.size * target_size / 2;
	int expanded_target_size = target_size + in.size * target_size;
	
	// �÷��̾ �»�� ��ǥ�ϱ� + �÷��̾� ũ���� ���� �� ���ش� (��� ��ǥ ���)
	if (RayVsRect(in.pos + in.size * target_size / 2, in.vel * fElapsedTime, expanded_target_pos, expanded_target_size,
		contact_point, contact_normal, contact_time))
	{
		// �浹 ������ 1�̸��̾�� �Ѵ�.
		// �»�ܿ��� target������ �� ��, �浹���� �ʾҴµ��� ������ �¾Ƽ� contact_time�� 1�̻����� ������ true�� �ȴ�. �� ��츦 ����.
		if (contact_time >= 0.0f && contact_time < 1.0f)
			return true;
	}

	return false;
}

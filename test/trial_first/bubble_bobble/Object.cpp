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

//참고: https://www.youtube.com/watch?v=8JJ-4JgR7Dg&list=LL&index=5
bool Object::RectVsRect(FPOINT & r1, int size1, FPOINT & r2, int size2)
{
	return (r1.x < r2.x + size2 && r1.x + size1 > r2.x && r1.y < r2.y + size2 && r1.y + size1 > r2.y);
}

bool Object::RayVsRect(FPOINT & ray_origin, FPOINT & ray_dir, FPOINT & target_pos, int target_size, 
	FPOINT & contact_point, FPOINT & contact_normal, float & t_hit_near)
{
	// 초기화
	contact_normal = { 0,0 };
	contact_point = { 0,0 };

	// ray_dir으로 나누기 위해 캐시해준다
	FPOINT invdir = 1.0f / ray_dir; 

	// ray_origin부터 ray_dir까지 time이 0 ~ 1이라고 가정했을 때,
	// Near tx, ty = target 사각형과 먼저 부딪힌 x지점, y지점이 ray_dir 대비 몇 %인지 (0 ~ +-1 사이의 값) = 그 지점의 시점 time이 된다.
	// Far tx, ty = 나중에 부딪힌
	FPOINT t_near = (target_pos - ray_origin) * invdir;
	FPOINT t_far = (target_pos + target_size - ray_origin) * invdir;

	// 0으로 나눴을 때 = ray_dir가 0일 때 = 숫자가 아닌값이 나온다 = isNan = true
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;
	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false; 

	// Near가 Far 보다 더 멀게 나왔다면 스왑해준다 (좌하단에서 우상단으로 가로지를 경우)
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	// 아래 경우만 사각형과 충돌이 일어난 경우다. 
	// Near.x < Far.y
	// Near.y < Far.x 
	// 이것을 제외한 경우는 false
	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

	// 가장 가깝게 충돌한 시점 t = Near.x Near.y 중에 큰 값
	t_hit_near = max(t_near.x, t_near.y);

	// 가장 멀게 충돌한 시점 t = Far.x Far.y 중에 작은 값
	float t_hit_far = min(t_far.x, t_far.y);

	// 가장 멀게 충돌한 시점이 마이너스다 = 가깝게 충돌한 시점도 마이너스다 = ay direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// 충돌 지점 = 원래위치 + 이동값 * 충돌 지점까지의 시간 t
	contact_point = ray_origin + ray_dir * t_hit_near;

	// 노멀벡터 알아내기
	// Near.x > Near.y 이고 ray_dir이 양수이면 x - 1
	//                                음수이면 x + 1
	// Near.x < Near.y 이고 ray_dir이 양수이면 y - 1
	//                                음수이면 y + 1 
	// Near.x = Near.y 이고 ray_dir.x ray_dir.y 
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
	// 가해진 힘이 없다면 false
	if (in.vel.x == 0 && in.vel.y == 0)
		return false;

	// 충돌 처리 후 플레이어 위치가 사각형 사이면 안되니까 사각형 크기를 늘려준다.
	// 사각형 위치 = 원래 위치 - 플레이어 크기의 반절
	// 사각형 크기 = 원래 크기 + 플레이어 크기
	FPOINT expanded_target_pos = target_pos - in.size * target_size / 2;
	int expanded_target_size = target_size + in.size * target_size;
	
	// 플레이어가 좌상단 좌표니까 + 플레이어 크기의 반절 을 해준다 (가운데 좌표 얻기)
	if (RayVsRect(in.pos + in.size * target_size / 2, in.vel * fElapsedTime, expanded_target_pos, expanded_target_size,
		contact_point, contact_normal, contact_time))
	{
		// 충돌 시점은 1미만이어야 한다.
		// 좌상단에서 target쪽으로 올 때, 충돌하지 않았는데도 조건이 맞아서 contact_time이 1이상으로 나오며 true가 된다. 이 경우를 방지.
		if (contact_time >= 0.0f && contact_time < 1.0f)
			return true;
	}

	return false;
}

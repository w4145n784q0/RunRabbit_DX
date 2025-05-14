#include "Character.h"

Character::Character(GameObject* parent)
	:GameObject(parent,"Character")
{
	//csv_.Load("CSVdata\\CharacterDataSample.csv");
}

Character::Character(GameObject* parent, const std::string& name):GameObject(parent, name)
{
}

Character::~Character()
{
}

void Character::SetcsvStatus(std::string _path)
{
	CsvReader csv;
	csv.Load(_path);
	

	StartPosition_.x = csv.GetValueFloat(1, 1);
	StartPosition_.y = csv.GetValueFloat(1, 2);
	StartPosition_.z = csv.GetValueFloat(1, 3);
	this->transform_.rotate_.x = csv.GetValueFloat(1, 4);
	this->transform_.rotate_.y = csv.GetValueFloat(1, 5);
	this->transform_.rotate_.z = csv.GetValueFloat(1, 6);
	this->transform_.scale_.x = csv.GetValueFloat(1, 7);
	this->transform_.scale_.y = csv.GetValueFloat(1, 8);
	this->transform_.scale_.z = csv.GetValueFloat(1, 9);
	FrontDirection_ = { csv.GetValueFloat(1, 10),csv.GetValueFloat(1, 11),csv.GetValueFloat(1, 12) };
	Velocity_ = csv.GetValueFloat(1, 13);
	
    Acceleration_ = csv.GetValueFloat(1, 14);
    AcceleValue_ = csv.GetValueFloat(1, 15);
    FullAccelerate_ = csv.GetValueFloat(1, 16);
	ForwardVector_ = { csv.GetValueFloat(1, 17), csv.GetValueFloat(1, 18), csv.GetValueFloat(1, 19) };

	MoveRotateX = csv.GetValueFloat(1, 20);
    FastRotateX = csv.GetValueFloat(1, 21);

    Gravity_ = csv.GetValueFloat(1, 22);
	IsOnGround_ = false;
    JumpSpeed_ = csv.GetValueFloat(1, 23);
	HeightLowerLimit_ = csv.GetValueFloat(1, 24);
	HeightUpperLimit_ = csv.GetValueFloat(1, 25);

    ColliderSize_ = csv.GetValueFloat(1, 26);
	KnockBack_Direction_ = { csv.GetValueFloat(1, 27), csv.GetValueFloat(1, 28) , csv.GetValueFloat(1, 29) };
    KnockBack_Velocity_ = { csv.GetValueFloat(1, 30), csv.GetValueFloat(1, 31) , csv.GetValueFloat(1, 32) };
	KnockBackPower = csv.GetValueFloat(1, 33);

    InvincibilityTime_ = csv.GetValueFloat(1, 34);
	IsInvincibility_ = false;
    InvincibilityValue = csv.GetValueFloat(1, 35);
}

void Character::CharacterGravity()
{
	JumpSpeed_ -= Gravity_;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	this->transform_.position_.y += JumpSpeed_;//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����

	if (this->transform_.position_.y <= HeightLowerLimit_)//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = HeightLowerLimit_;
		IsOnGround_ = true;
	}
}

void Character::CharacterMove(XMVECTOR _direction)
{
	//�x����]����
	XMVECTOR prev = RotateVecFront(this->transform_.rotate_.y, _direction);

	//�P�ʃx�N�g�������A�ړ��������m��
	MoveDirection_ = XMVector3Normalize(prev);

	CreateMoveVector();

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, NewPositon_);
	if (!(tmp.x > 60.0f || tmp.x < -60.0f || tmp.z > 60.0f || tmp.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPositon_);
	}
}

void Character::CreateMoveVector()
{
	//�ړ��x�N�g�� = �ړ����� * ((�����x + �����x) * 1f�̈ړ��ʂ̃X�P�[�����O)
	//�ړ��x�N�g��������
	XMVECTOR MoveVector = XMVectorScale(MoveDirection_, (Velocity_ + Acceleration_) * DeltaTime);

	//���݈ʒu�ƈړ��x�N�g�������Z
	XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	NewPositon_ = PrevPos + MoveVector;
}

XMVECTOR Character::RotateVecFront(float rotY, XMVECTOR front)
{
	//��]���������x�N�g���i�����j����
	//��{�̓��[�J�����ʃx�N�g��
	XMVECTOR v = front;

	//Y��]�����W�A���ɂ��A��]�s����쐬
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(rotY));

	//�����x�N�g������]�s��ŕϊ����A���[���h���W�ł̌����Ă���������o��
	v = XMVector3TransformCoord(v, m);

	return v;
}

XMVECTOR Character::CalclationForward(float rotY, XMVECTOR front)
{
	XMVECTOR v = front;
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(rotY));
	XMVECTOR forward = XMVector3TransformNormal(v,m);
	return  forward;
}

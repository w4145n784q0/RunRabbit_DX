#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"

//�v���C���[,�G�N���X�̋��ʎ����N���X
class Character :
    public GameObject
{
private:
    CsvReader csv;//csv��ǂݍ��ރC���X�^���X
protected:
    //----------�������----------
    XMFLOAT3 StartPosition_;//�����ʒu
    XMVECTOR FrontDirection_;//���ʂ̏����l ��������ǂꂾ����]������

    //----------�ړ�----------
    float Velocity_;//�����x ���̑��x�ɉ����x�����Z�����
    float Acceleration_;//�����x
    float AcceleValue_;//Acceleration_�㏸���A1f������̒~�ϗ�
    float FullAccelerate_;//�����x�̍ő�
    XMVECTOR ForwardVector_;//�L�����N�^�[���猩�����ʂ̕���

    //----------��]----------
    float MoveRotateX;//�ړ�����1f�̉�]��
    float FastRotateX;//(�`���[�W���Ȃ�)������]����1f�̉�]��

    //----------��----------
    float Gravity_; //�d�� �L�����N�^�[�̉������ɂ������
    float JumpSpeed_; //�v���C���[�̏�����Ɍ����� +�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����

    //----------��e----------
    float ColliderSize_; //�����蔻��(����)�̃T�C�Y
    XMFLOAT3 KnockBack_Direction_;//�m�b�N�o�b�N�������
    XMFLOAT3 KnockBack_Velocity_;//�m�b�N�o�b�N���鑬�x
    float KnockBackPower; //�m�b�N�o�b�N���鋭��

    //----------�ǂ̐ڐG�_���[�W----------
    int InvincibilityTime_;//�_���[�W��̖��G���� 1f���ƂɌ���
    bool IsInvincibility_;//���G���Ԃ�
    int InvincibilityValue;//���G���Ԃ̒l�@���̒l�𒴂���Ɩ��G���ԏI��

public:
    Character(GameObject* parent);
    ~Character();
    
    XMFLOAT3 GetStartPosition();
};


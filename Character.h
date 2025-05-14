#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"

//�v���C���[,�G�N���X�̋��ʎ����N���X
class Character :
    public GameObject
{
private:
    //CsvReader csv_;//csv��ǂݍ��ރC���X�^���X
protected:
    //----------�������----------
    XMFLOAT3 StartPosition_;//�����ʒu
    XMVECTOR FrontDirection_;//���ʂ̏����l(���[�J�����W�n) ��������ǂꂾ����]������

    //----------�ړ�----------
    float Velocity_;//�����x ���̑��x�ɉ����x�����Z�����
    float Acceleration_;//�����x
    float AcceleValue_;//Acceleration_�㏸���A1f������̑�����
    float FullAccelerate_;//�����x�̍ő�
    XMVECTOR ForwardVector_;//�L�����N�^�[���猩�����ʂ̕���(���[���h���W�n) ���g��y����]�ʂƂ����Čv�Z
    XMVECTOR MoveDirection_;//�ړ����� ���̒l�ɑ����̗v�f�������Ĉړ��x�N�g��������
    XMVECTOR NewPositon_;//�ړ���̃x�N�g��

    //----------��]----------
    float MoveRotateX;//�ړ�����1f�̉�]��
    float FastRotateX;//(�`���[�W���Ȃ�)������]����1f�̉�]��

    //----------��----------
    float Gravity_; //�d�� �L�����N�^�[�̉������ɂ������
    bool IsOnGround_;//�n�ʂɂ��邩
    float JumpSpeed_; //�v���C���[�̏�����Ɍ����� +�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
    float HeightLowerLimit_;//�����̉���
    float HeightUpperLimit_;//�����̏��


    //----------��e----------
    float ColliderSize_; //�����蔻��(����)�̃T�C�Y
    XMFLOAT3 KnockBack_Direction_;//�m�b�N�o�b�N�������
    XMFLOAT3 KnockBack_Velocity_;//�m�b�N�o�b�N���鑬�x
    float KnockBackPower; //�m�b�N�o�b�N���鋭��

    //�m�b�N�o�b�N���鑬�x= �m�b�N�o�b�N���鋭��(�萔) * �m�b�N�o�b�N�������

    //----------�ǂ̐ڐG�_���[�W----------
    int InvincibilityTime_;//�_���[�W��̖��G���� 1f���ƂɌ���
    bool IsInvincibility_;//���G���Ԃ�
    int InvincibilityValue;//���G���Ԃ̒l�@���̒l�𒴂���Ɩ��G���ԏI��

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    /// <summary>
    /// CSV����e�X�e�[�^�X��ǂݍ��݂���
    /// Character�N���X����
    /// </summary>
    /// <param name="_path">csv�t�@�C���̃p�X</param>
    void SetcsvStatus(std::string _path);

    //----------���ʏ���----------

    /// <summary>
    /// �����ʒu�̐ݒ�
    /// </summary>
    void SetStartPosition() { this->transform_.position_ = StartPosition_; }

    /// <summary>
    /// �d�͏���
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// �L�����N�^�[�̈ړ�����
    /// </summary>
    /// <param name="_direction">�����������x�N�g��</param>
    void CharacterMove(XMVECTOR _direction);

    /// <summary>
    /// �ړ��x�N�g��������
    /// </summary>
    void CreateMoveVector();

    /// <summary>
    /// Y���̉�]�s����x�N�g���ɕϊ�
    /// </summary>
    /// <param name="rotY">Y���ɉ�]�������p�x�iDegree�j</param>
    /// <param name="front">���ʃx�N�g��(���[�J�����)</param>
    /// <returns>�ό`�����x�N�g���i���[���h��ԁj</returns>
    XMVECTOR RotateVecFront(float rotY, XMVECTOR front);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="rotY"></param>
    /// <param name="front"></param>
    /// <returns></returns>
    XMVECTOR CalclationForward(float rotY, XMVECTOR front);

};


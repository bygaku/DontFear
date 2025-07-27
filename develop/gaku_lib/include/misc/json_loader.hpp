#pragma once
#include <nlohmann/json.hpp>
#include "data/json_file_type.hpp"

using json = nlohmann::json;

class JsonLoader
{
public:
	/// @brief �C���X�^���X�𐶐�
	static void Create()  noexcept;

	/// @brief �C���X�^���X��j��
	static void Destroy() noexcept;

	/// @brief �C���X�^���X���擾
	static JsonLoader* GetInstance() noexcept { return instance_; }

	/// @brief �R���t�B�O�f�[�^���擾
	json GetConfigData() const;

	/// @brief �Z�[�u�f�[�^���擾
	json GetSaveData()	 const;

	/// @brief  string�^��wstring�^�ϊ�
	/// @param	str: �ϊ����镶����
	/// @return	�ϊ�����������
	std::wstring Widening(const std::string& str) noexcept;

private:
	/// @brief �C���X�^���X��������
	JsonLoader();

	/// @brief �C���X�^���X�����
	~JsonLoader();

	/// @brief ���[�h����������
	bool IsLoadingFile();

private:
	std::unordered_map<std::string, json>	json_data_;		/// @brief Json�̃f�[�^
	static		 JsonLoader*				instance_;		/// @brief �N���X�̃C���X�^���X

private:
	static const std::string config_data_;	/// @brief config_data.json�̃t�@�C���p�X
	static const std::string save_data_;	/// @brief save_data.json�̃t�@�C���p�X

};


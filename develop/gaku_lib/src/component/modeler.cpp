#include <DxLib.h>
#include "component/modeler.hpp"

Modeler::Modeler(GameObject& owner)
	: Component{ owner }
	, model_handle_(-1)
{
};

Modeler::~Modeler()
{
	if (!model_handle_) return;
	MV1DeleteModel(model_handle_);
}

void Modeler::Initialize() noexcept
{
}

void Modeler::Draw() const noexcept
{
	if (!model_handle_) return;
	MV1DrawModel(model_handle_);
}

int Modeler::GetModelHandle() const noexcept
{
	return model_handle_;
}

void Modeler::SetModelHandle(std::string file_path) noexcept
{
	if (model_handle_ && model_handle_ != -1) return;
	/// TODO: ResourceManagerÇ…ìoò^å„ÅAéÊìæ
	model_handle_ = MV1LoadModel(file_path.c_str());
}

void Modeler::SetModelHandle(int model_handle) noexcept
{
	if (model_handle_ && model_handle_ != -1) return;
	model_handle_ = model_handle;
}

void Modeler::ResetModelHandle() noexcept
{
	if (!model_handle_) return;
	MV1DeleteModel(model_handle_);
	model_handle_ = -1;
}

void Modeler::SetMatrix(MATRIX mat) const noexcept
{
	if (!model_handle_) return;

	// Yé≤Ç≈180ìxâÒì]
	//MATRIX modelRotation = MGetRotY(DX_PI_F);
	//MATRIX finalMatrix   = MMult(modelRotation, mat);

	MV1SetMatrix(model_handle_, mat);
}

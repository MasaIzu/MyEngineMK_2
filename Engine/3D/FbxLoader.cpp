#include "FbxLoader.h"

#include <cassert>

#include "Mesh.h"
#include "Material.h"
#include "TextureManager.h"

using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";
FbxLoader* FbxLoader::fbxLoader_;

FbxLoader* FbxLoader::GetInstance()
{
	if (fbxLoader_ == nullptr)
	{
		fbxLoader_ = new FbxLoader();
	}

	return fbxLoader_;
}

//void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src) {
//
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
//		}
//	}
//
//}

void FbxLoader::Initialize(ID3D12Device* device)
{
	// �������烁���o�ϐ��ɑ��
	this->device = device;

}


void FbxLoader::Finalize()
{
	delete fbxLoader_;
}

FbxModel* FbxLoader::LoadModelFromFile(const string& modelName)
{
	// ���f���Ɠ������O�̃t�H���_����ǂݍ���
	const string directoryPath = baseDirectory + modelName + "/";
	// �g���q.fbx��t��
	const string fileName = modelName + ".fbx";
	// �A�����ăt���p�X�𓾂�
	const string fullpath = directoryPath + fileName;

	mScene = aiImportFile(fullpath.c_str(), flag);

	// �t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
	if (!mScene) {
		assert(0);
	}


	// ���f������
	FbxModel* model = new FbxModel();
	model->name_ = modelName;

	// FBX�m�[�h�̐����擾
	UINT32 nodeCount = 0;
	GetNodeNum(mScene->mRootNode, nodeCount);

	// ���炩���ߕK�v�����̃��������m�ۂ��邱�ƂŁA�A�h���X�������̂�\�h
	model->nodes.reserve(nodeCount);

	model->globalInverseTransform = MyMath::AssimpMatrix(mScene->mRootNode->mTransformation);


	// ���[�g�m�[�h���珇�ɉ�͂��ă��f���ɗ�������
	ParseNodeRecursive(model, mScene->mRootNode);
	// FBX�V�[�����
	aiReleaseImport(mScene);

	for (size_t i = 0; i < model->nodes.size(); i++)
	{
		if (model->nodes[i].parent)
		{
			auto itr = std::find_if(model->nodes.begin(), model->nodes.end(), [&](Node& node)
				{
					return node.name == model->nodes[i].parent->name;
				});

			itr->childrens.push_back(&model->nodes[i]);
		}
	}

	model->SetTextureHandle(textureHandle);

	return model;
}

void FbxLoader::ParseSkin(FbxModel* model, aiMesh* fbxMesh) {

	auto& vertices = model->meshes_.back()->vertices_;

	struct WeightSet {
		UINT index;
		float weight;
	};

	std::vector<std::list<WeightSet>> weightLists(vertices.size());

	if (fbxMesh->mNumBones == 0) {
		return;
	}

	for (unsigned int i = 0; i < fbxMesh->mNumBones; i++) {


		auto& meshBone = fbxMesh->mBones[i];

		//�{�[�����̂̃m�[�h�̖��O���擾
		const char* boneName = meshBone->mName.C_Str();

		//�V�����{�[����ǉ����A�ǉ������{�[���̎Q�Ƃ𓾂�
		Mesh::Bone bone;

		bone.name = boneName;

		//FBX���珉���p���s����擾����

		//�����p���s��̋t�s��𓾂�
		bone.offsetMatirx = MyMath::AssimpMatrix(meshBone->mOffsetMatrix.Transpose());



		bone.index = i;

		model->meshes_.back()->vecBones.push_back(bone);
		//model->meshes_.back()->bones[bone.name] = &model->meshes_.back()->vecBones.back();

		for (unsigned int j = 0; j < meshBone->mNumWeights; j++) {
			int vertIndex = meshBone->mWeights[j].mVertexId;

			float weight = (float)meshBone->mWeights[j].mWeight;

			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}
	}

	//�e���_�ɂ��ď���
	for (size_t j = 0; j < vertices.size(); j++)
	{
		//���_�̃E�F�C�g����ł��傫��4��I��
		auto& weightList = weightLists[j];

		size_t weightArrayIndex = 0;
		//�~���\�[�g�ς݂̃E�F�C�g���X�g����

		for (auto& weightSet : weightList)
		{
			//���_�f�[�^�ɏ�������
			vertices[j].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[j].boneWeight[weightArrayIndex] = weightSet.weight;

			//4�ɒB������C��
			if (++weightArrayIndex >= Mesh::MAX_BONE_INDICES)
			{
				break;
			}

		}
	}

}

void FbxLoader::ParseNodeRecursive(FbxModel* model, aiNode* fbxNode, Node* parent)
{
	//// �m�[�h�����擾
	//string name = fbxNode->GetName();
	// ���f���Ƀm�[�h��ǉ�
	model->nodes.emplace_back();
	Node& node = model->nodes.back();
	// �m�[�h�����擾
	node.name = fbxNode->mName.C_Str();

	node.transform =
	{
		fbxNode->mTransformation.a1, fbxNode->mTransformation.a2, fbxNode->mTransformation.a3, fbxNode->mTransformation.a4,
		fbxNode->mTransformation.b1, fbxNode->mTransformation.b2, fbxNode->mTransformation.b3, fbxNode->mTransformation.b4,
		fbxNode->mTransformation.c1, fbxNode->mTransformation.c2, fbxNode->mTransformation.c3, fbxNode->mTransformation.c4,
		fbxNode->mTransformation.d1, fbxNode->mTransformation.d2, fbxNode->mTransformation.d3, fbxNode->mTransformation.d4,
	};

	//�O���[�o���ό`�s��̌v�Z
	node.globalTransform = node.transform;

	if (parent) {
		node.parent = parent;
		// �e�̕ό`����Z
		node.globalTransform *= parent->globalTransform;
	}

	for (UINT32 i = 0; i < fbxNode->mNumMeshes; i++) {
		aiMesh* aimesh = mScene->mMeshes[fbxNode->mMeshes[i]];
		if (aimesh) {
			model->meshes_.emplace_back();
			model->meshes_.back() = new Mesh();

			model->meshes_.back()->name_ = aimesh->mName.C_Str();

			model->meshes_.back()->node = &node;

			ParseMesh(model, aimesh);
		}
	}

	// �q�m�[�h�ɑ΂��čċA�Ăяo��
	for (unsigned int i = 0; i < fbxNode->mNumChildren; i++) {
		ParseNodeRecursive(model, fbxNode->mChildren[i], &node);
	}
}

void FbxLoader::ParseMesh(FbxModel* model, aiMesh* fbxMesh)
{

	// ���_���W�ǂݎ��
	ParseMeshVertices(model, fbxMesh);
	// �ʂ��\������f�[�^�̓ǂݎ��
	ParseMeshFaces(model, fbxMesh);
	// �}�e���A���̓ǂݎ��
	ParseMaterial(model, fbxMesh, mScene->mMaterials[fbxMesh->mMaterialIndex]);

	if (fbxMesh->HasBones()) {
		//�X�L�j���O���̓ǂݎ��
		ParseSkin(model, fbxMesh);
	}


}

void FbxLoader::ParseMeshVertices(FbxModel* model, aiMesh* fbxMesh)
{
	auto& vertices = model->meshes_.back()->vertices_;

	// ���_���W�f�[�^�̐�
	const int controlPointsCount = fbxMesh->mNumVertices;

	// �K�v���������_�f�[�^�z����m��
	Mesh::VertexPosNormalUv vert{};
	model->meshes_.back()->vertices_.resize(controlPointsCount, vert);


	// FBX���b�V���̑S���_���W�����f�����̔z��ɃR�s�[����B
	for (int i = 0; i < controlPointsCount; i++) {
		Mesh::VertexPosNormalUv& vertex = vertices[i];
		// ���W�̃R�s�[
		aiVector3D position = fbxMesh->mVertices[i];

		vertex.pos.x = position.x;
		vertex.pos.y = position.y;
		vertex.pos.z = position.z;

		// �@���̃R�s�[
		aiVector3D normal = fbxMesh->mNormals[i];

		vertex.normal.x = normal.x;
		vertex.normal.y = normal.y;
		vertex.normal.z = normal.z;

	}
}

void FbxLoader::ParseMeshFaces(FbxModel* model, aiMesh* fbxMesh)
{
	auto& vertices = model->meshes_.back()->vertices_;
	auto& indices = model->meshes_.back()->indices_;

	// �ʂ̐�
	const int polygonCount = fbxMesh->mNumVertices;

	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	// �ʂ��Ƃ̏��ǂݎ��
	for (int i = 0; i < polygonCount; i++) {
		aiVector3D* uv = (fbxMesh->HasTextureCoords(0)) ? &(fbxMesh->mTextureCoords[0][i]) : &zero3D;

		vertices[i].uv = Vector2(uv->x, -uv->y);
	}

	indices.resize(fbxMesh->mNumFaces * 3);

	for (UINT i = 0; i < fbxMesh->mNumFaces; i++) {
		const aiFace& face = fbxMesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++) {
			indices[i * 3 + j] = face.mIndices[j];
		}

	}

}

void FbxLoader::ParseMaterial(FbxModel* model, aiMesh* fbxMesh, aiMaterial* aimaterial)
{

	auto& material = model->meshes_.back()->material_;

	material = Material::Create();

	aiColor3D ambient(0.3f, 0.3f, 0.3f);
	aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	material->ambient_ = Vector3(ambient.r, ambient.g, ambient.b);

	aiColor3D diffuse(0.0f, 0.0f, 0.0f);
	aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	material->diffuse_ = Vector3(diffuse.r, diffuse.g, diffuse.b);

	aiColor3D specular(0.0f, 0.0f, 0.0f);
	aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	material->specular_ = Vector3(specular.r, specular.g, specular.b);

	aiString str;
	aimaterial->Get(AI_MATKEY_NAME, str);
	material->name_ = str.C_Str();

	uint32_t deffuseMap = LoadMatrixerialTextures(aimaterial, aiTextureType_DIFFUSE, "Diffuse", mScene, model->name_);

	material->SetTextureHadle(deffuseMap);

}


std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;
	// ��؂蕶�� '\\' ���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind('\\');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// ��؂蕶�� '/' ���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind('/');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}

void FbxLoader::GetNodeNum(const aiNode* node, UINT32& num)
{
	if (node->mChildren)
	{
		for (UINT32 i = 0; i < node->mNumChildren; i++)
		{
			GetNodeNum(node->mChildren[i], num);
		}
	}

	num++;
}

uint32_t FbxLoader::LoadMatrixerialTextures(aiMaterial* cmatrix, aiTextureType type, std::string typeName, const aiScene* scene_, const std::string& modelName)
{
	uint32_t textures;

	for (size_t i = 0; i < cmatrix->GetTextureCount(type); i++)
	{
		aiString str;
		cmatrix->GetTexture(type, static_cast<UINT>(i), &str);
		uint32_t texture;
		{
			std::string filename = ExtractFileName(std::string(str.C_Str()));
			filename = modelName + '\\' + filename;
			texture = TextureManager::Load(filename);
			textureHandle = texture;
		}

		textures = texture;
	}
	return textures;
}
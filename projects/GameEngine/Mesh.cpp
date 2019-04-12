#include "Mesh.h"


Mesh::Mesh() {
}

Mesh::Mesh(std::string filepath) {
	this->VerticesIdx = std::vector<unsigned int>();
	this->LoadFromFile(filepath);
	this->HasTexs = (this->TexsIdx.size() > 0);
	this->HasNormals = (this->NormalsIdx.size() > 0);
	this->ProcessMesh();

	this->CreateBufferObjects();
	this->FreeMeshData();

}

void Mesh::FreeMeshData() {
	this->VerticesData.clear();
	this->TexsData.clear();
	this->NormalsData.clear();
	this->VerticesIdx.clear();
	this->TexsIdx.clear();
	this->NormalsIdx.clear();
}

void Mesh::ProcessMesh() {
	for (auto i = 0; i < this->VerticesIdx.size(); i++) {
		auto vi = this->VerticesIdx[i];
		Vertex v = this->VerticesData[vi - 1];
		this->Vertices.push_back(v);
		if (this->HasTexs) {
			auto ti = this->TexsIdx[i];
			auto t = this->TexsData[ti - 1];
			this->Texs.push_back(t);
		}
		if (this->HasNormals) {
			auto ni = this->NormalsIdx[i];
			auto n = this->NormalsData[ni - 1];
			this->Normals.push_back(n);
		}
	}
	if (this->HasTexs && this->HasNormals)
		this->CalculateTangents();
}

void Mesh::Draw() {
	glBindVertexArray(this->VaoId);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->Vertices.size());
	glBindVertexArray(0);
}


Mesh::~Mesh() {

}

void Mesh::parseVertexs(std::stringstream& in) {
	Vertex v;
	in >> v;
	//std::cout <<"Vertex: "<< v << std::endl;
	this->VerticesData.push_back(v);
}

void Mesh::parseTextures(std::stringstream& in) {
	TexCoordinates t;
	in >> t;
	//std::cout << t << std::endl;
	this->TexsData.push_back(t);

}

void Mesh::parseNormals(std::stringstream& in) {
	Normal n;
	in >> n;
	//std::cout <<"Normal: " << n << std::endl;
	this->NormalsData.push_back(n);

}

void Mesh::parseFaces(std::stringstream& in) {
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(in, token, '/');
		if (token.size() > 0)
			this->VerticesIdx.push_back(std::stoi(token));
		std::getline(in, token, '/');
		if (token.size() > 0)
			this->TexsIdx.push_back(std::stoi(token));
		std::getline(in, token, ' ');
		if (token.size() > 0)
			this->NormalsIdx.push_back(std::stoi(token));
	}
	//std::cout << in.str() << std::endl;
}

void Mesh::parseComments(std::stringstream& in) {
	std::cout << in.str() << std::endl;
}

bool Mesh::CreateBufferObjects() {
	glGenVertexArrays(1, &this->VaoId);
	glBindVertexArray(this->VaoId);
	{
		glGenBuffers(1, &this->VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, this->VboVertices);
		glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTEX);
		glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		if (this->HasTexs) {
			glGenBuffers(1, &this->VboTextures);
			glBindBuffer(GL_ARRAY_BUFFER, this->VboTextures);
			glBufferData(GL_ARRAY_BUFFER, this->Texs.size() * sizeof(TexCoordinates), &Texs[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXCOORDS);
			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(TexCoordinates), 0);
		}
		if (this->HasNormals) {
			glGenBuffers(1, &this->VboNormals);
			glBindBuffer(GL_ARRAY_BUFFER, this->VboNormals);
			glBufferData(GL_ARRAY_BUFFER, this->Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMALS);
			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
		}
		if (this->HasTangents) {
			glGenBuffers(1, &this->VboTangents);
			glBindBuffer(GL_ARRAY_BUFFER, this->VboTangents);
			glBufferData(GL_ARRAY_BUFFER, this->Tangents.size() * sizeof(Vector4), &Tangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TANGENTS);
			glVertexAttribPointer(TANGENTS, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
		}
		if (this->HasBiTangents) {
			glGenBuffers(1, &this->VboBiTangents);
			glBindBuffer(GL_ARRAY_BUFFER, this->VboBiTangents);
			glBufferData(GL_ARRAY_BUFFER, this->BiTangents.size() * sizeof(Vector3), &BiTangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(BITANGENTS);
			glVertexAttribPointer(BITANGENTS, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
		}


		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return true;
}

void Mesh::LoadFromFile(std::string filename) {

	std::ifstream file = std::ifstream(filename);
	if (file.fail()) {
		std::string error = "The mesh " + filename + " seems to be missing.";
		throw std::ios_base::failure(error);
		this->Error = true;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		this->ParseLine(std::stringstream(line));
	}
	file.close();

}

void Mesh::ParseLine(std::stringstream& in) {
	std::string s;
	in >> s;
	if (s == "v")
		parseVertexs(in);
	else if (s == "vt")
		parseTextures(in);
	else if (s == "vn")
		parseNormals(in);
	else if (s == "f")
		parseFaces(in);
	else if (s == "#")
		parseComments(in);

}


void Mesh::CalculateTangents() {
	Vector4* tan1 = new Vector4[this->Vertices.size() * 2];
	Vector4* tan2 = tan1 + this->Vertices.size();
	ZeroMemory(tan1, this->Vertices.size() * sizeof(Vector3) * 2);

	for (int i = 0; i < this->Vertices.size(); i += 3) {
		Vector3 v0 = this->Vertices[i + 0];
		Vector3 v1 = this->Vertices[i + 1];
		Vector3 v2 = this->Vertices[i + 2];

		Vector2 uv0 = this->Texs[i + 0];
		Vector2 uv1 = this->Texs[i + 1];
		Vector2 uv2 = this->Texs[i + 2];

		Vector3 deltaPos1 = v1 - v0;
		Vector3 deltaPos2 = v2 - v0;

		Vector2 deltaUV1 = uv1 - uv0;
		Vector2 deltaUV2 = uv2 - uv0;


		float r = 1.0f / (deltaUV1.coordinates[0] * deltaUV2.coordinates[1] - deltaUV1.coordinates[1] * deltaUV2.coordinates[0
		]);

		Vector3 tangent = (deltaPos1 * deltaUV2.coordinates[1] - deltaPos2 * deltaUV1.coordinates[1]) * r;

		Vector3 bitangent = (deltaPos2 * deltaUV1.coordinates[0] - deltaPos1 * deltaUV2.coordinates[0]) * r;

		this->Tangents.push_back(tangent);
		this->Tangents.push_back(tangent);
		this->Tangents.push_back(tangent);

		this->BiTangents.push_back(bitangent);
		this->BiTangents.push_back(bitangent);
		this->BiTangents.push_back(bitangent);

	}

	for (long a = 0; a < this->Vertices.size(); a++) {
		Vector3 n = this->Normals[a];
		Vector4 t = this->Tangents[a];
		Vector3 t3d = Vector3(t.coordinates[0], t.coordinates[1], t.coordinates[2]);
		//Gram Schmidt orthogonalize
		this->Tangents[a] = ((t3d - (n * t3d) * n)).normalize();
		Vector3 tan2_3d = Vector3(tan1[this->Vertices.size() + a].coordinates[0],
		                          tan1[this->Vertices.size() + a].coordinates[1],
		                          tan1[this->Vertices.size() + a].coordinates[2]);
		this->Tangents[a].coordinates[3] = ((Vector3::crossProduct(n, t3d) * tan2_3d) < 0.0f) ? -1.0f : 1.0f;
	}
	this->HasTangents = (this->Tangents.size() > 0);
	this->HasBiTangents = (this->BiTangents.size() > 0);

	delete[] tan1;

}

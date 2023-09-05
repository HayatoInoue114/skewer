#pragma once
#include <Matrix4x4.h>
#include <Vector2.h>
#include <Vector3.h>
#include <time.h>

int GetRandom(int min, int max);

//float GetRandom(float min, float max);

float Clamp(float number, float min, float max);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ(ノルム)
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);

static const int kColumnWidth = 60;
// void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

// 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

float InverseNum(const Matrix4x4& m, int a, int b, int c, int d, int e, int f, int g, int h);
float InverseNum2(const Matrix4x4& m, int a, int b, int c, int d, int e, int f);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 単位行列の作成
Matrix4x4 MakeIdentity4x4();

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix);

// 1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
// 2.正射影行列
Matrix4x4 MakeOrthographicMatrix(
    float left, float right, float top, float bottom, float nearClip, float farClip);
// 3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

//行列とベクトルの掛け算
Vector3 Multyply(const Vector3& v, const Matrix4x4& m);

Vector3 Transform(const Vector3& num, const Matrix4x4& m);

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

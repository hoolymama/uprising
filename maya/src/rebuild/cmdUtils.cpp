#include "cmdUtils.h"

void CmdUtils::flatten(const MFloatMatrix &mat, MDoubleArray &result) {
	result.clear();
	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 4; ++c)
		{
			result.append(double(mat[r][c]));
		}
	}
}

void CmdUtils::flatten(const MVectorArray &arr, MDoubleArray &result) {

	unsigned len = arr.length();
	result.setLength(len * 3);
	unsigned j = 0;
	for (int i = 0; i < len; ++i)
	{
		result.set(arr[i].x, j++);
		result.set(arr[i].y, j++);
		result.set(arr[i].z, j++);
	}
}

void CmdUtils::flatten(const MPointArray &arr, MDoubleArray &result) {

	unsigned len = arr.length();
	result.setLength(len * 3);
	unsigned j = 0;
	for (int i = 0; i < len; ++i)
	{
		result.set(arr[i].x, j++);
		result.set(arr[i].y, j++);
		result.set(arr[i].z, j++);
	}
}


void CmdUtils::flatten(const MFloatPointArray &arr, MDoubleArray &result) {

	unsigned len = arr.length();
	result.setLength(len * 3);
	unsigned j = 0;
	for (int i = 0; i < len; ++i)
	{
		result.set(double(arr[i].x), j++);
		result.set(double(arr[i].y), j++);
		result.set(double(arr[i].z), j++);
	}
}

void CmdUtils::flatten(const MVector &v, MDoubleArray &result) {
	result.clear();
	result.append(v.x);
	result.append(v.y);
	result.append(v.z);
}
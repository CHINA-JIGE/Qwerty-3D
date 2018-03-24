
/***********************************************************************

                           �ࣺNOISE File Manager

			�����������ļ���������ͬ�ļ����Ƿ��ڲ�ͬ��cppʵ�ְɣ�
			��Զ���һ��

************************************************************************/

#include "Noise3D.h"

using namespace Noise3D;

/*
ios::in 		��
ios::out		д
ios::app		���ļ�ĩβ��ʼд
ios::binary       ������ģʽ
ios::nocreate	��һ���ļ�ʱ������ļ������ڣ��������ļ���
ios::noreplace	��һ���ļ�ʱ������ļ������ڣ��������ļ�
ios::trunc		��һ���ļ���Ȼ���������
ios::ate		��һ���ļ�ʱ����λ���ƶ����ļ�β
*/

IFileManager::IFileManager()
{

};

BOOL IFileManager::ImportFile_PURE(NFilePath pFilePath, std::vector<char>& byteBuffer)
{
	//�ļ�������
	std::ifstream fileIn(pFilePath, std::ios::binary);

	//�ļ������ھ�return
	if (!fileIn.is_open()) 
	{
		ERROR_MSG("FileManager : Cannot Open File !!");
		return FALSE;
	}

	//ָ���Ƶ��ļ�β
	fileIn.seekg(0, std::ios_base::end);

	//ָ��ָ���ļ�β����ǰλ�þ��Ǵ�С
	int static_fileSize = (int)fileIn.tellg();


	//ָ���Ƶ��ļ�ͷ
	fileIn.seekg(0, std::ios_base::beg);

	//...........
	int i = 0;char tmpC =0;

	//allocate new memory block , initialized with 0
	byteBuffer.resize(static_fileSize,0);
	while (!fileIn.eof())
	{
		
		fileIn.read(&byteBuffer.at(0), static_fileSize);
		//���ֽڶ�ȡ
		//fileIn.get(tmpC);
		//pFileBuffer->push_back(tmpC);
	}


	//�ر��ļ�
	fileIn.close();


	return TRUE;
}

BOOL IFileManager::ExportFile_PURE(NFilePath pFilePath, std::vector<char>* pFileBuffer, BOOL canOverlapOld)
{

	std::ofstream fileOut;

	//can we overlap the old file??
	if(canOverlapOld)
	{
		fileOut.open(pFilePath, std::ios::binary | std::ios::trunc);
	}
	else
	{
		fileOut.open(pFilePath, std::ios::binary | std::ios::app);
	}

	//check if we have successfully opened the file
	if (!fileOut.good())
	{
		ERROR_MSG("FileManager : Cannot Open File !!");
		return FALSE;
	}

	//...........
	UINT i = 0;char tmpC = 0;
	for (i = 0;i < pFileBuffer->size();i++)
	{
		fileOut.put(pFileBuffer->at(i));
	}

	//�ر��ļ�
	fileOut.close();

	return TRUE;
}

//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2016/02/16
//��˵����ָ�빤�߼�
//*************************************************
#pragma once

#ifndef NULL
#define NULL 0
#endif 

namespace CkPtrUtils
{
	/*
	** �뿪������ʱ,�Զ��ͷ�ָ��,����ָ�븳ֵΪNULL
	** ��: 
	** Type* t = new Type;
	** CkPtrUtils::PtrScopeGuard<Type> pag(&t, false);
	** Type* t = new Type[N];
	** CkPtrUtils::PtrScopeGuard<Type> pag(&t, true);
	** @Param bIsArr: �Ƿ���ָ�������ָ��,�� true, �� false
	*/
	template<typename T>
	class PtrScopeGuard
	{
		T** m_ptr;
		bool m_bIsArr;
	public:
		PtrScopeGuard(T** ptr, bool bIsArr)
			:m_ptr(ptr),
			m_bIsArr(bIsArr)
		{
		};
		~PtrScopeGuard()
		{
			if (m_bIsArr)
				delete[](*m_ptr);
			else
				delete (*m_ptr);
			(*m_ptr) = NULL;
		};
	};
};

// Map.h
// リスト
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __MAP_H__
#define  __MAP_H__


/******************************************************************************/
//		インクルード
/******************************************************************************/

#include "..\include\include.h"
#include "..\include\stl.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

template <class ID, class DATA> class Map
{
protected:
	map<ID, DATA> mapItem;

public:
	static const UINT npos;

	Map();
	Map(const Map&);
	~Map();

// 基本動作
private:
	map<ID, DATA>::iterator GetIterator(const ID&) const;
	BOOL IsValidIterator(map<ID, DATA>::iterator p) const;

public:
	UINT GetSize() const;
	BOOL IsInMap(ID) const;

	virtual BOOL Insert(ID, DATA, UINT ui = 0);
	virtual BOOL Delete(UINT);
	virtual BOOL Delete(const ID&);

	BOOL Clear();

	const data at(ID) const;
	data at(ID);

// 付加動作
	BOOL Move(UINT, UINT);

	const data operator[](ID) const;
	data operator[](ID);

	UINT Find(const DATA&, UINT uiIndexStart = 0) const;
};



/******************************************************************************/
//		定義
/******************************************************************************/

template <class ID, class DATA> UINT Map<class ID, class DATA>::npos = -1;


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> Map<class ID, class DATA>::Map()
{
}


/******************************************************************************/
// コピーコンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> Map<class ID, class DATA>::Map(const Map& l)
{
	mapItem = l.mapItem;
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> Map<class ID, class DATA>::~Map()
{
}



/******************************************************************************/
//		基本動作
/******************************************************************************/
// リストの中のイテレータを取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> map<ID, DATA>::iterator Map<class ID, class DATA>::GetIterator(const ID& id) const
{
	map<ID, DATA>::iterator i = mapItem.find(id);
	return i;
}


/******************************************************************************/
// イテレータが正しい値か
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> BOOL Map<class ID, class DATA>::IsValidIterator(map<ID, DATA>::iterator p) const
{
	return p != mapItem.end();
}


/******************************************************************************/
// リストの中にあるか
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> BOOL Map<class ID, class DATA>::IsInMap(const string& id) const
{
	return IsValidIterator(GetIterator(id));
}


/******************************************************************************/
// サイズ取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> UINT Map<class ID, class DATA>::GetSize() const
{
	return mapItem.size();
}


/******************************************************************************/
// 追加
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> BOOL Map<class ID, class DATA>::Insert(const ID& id, const DATA& data)
{
	if(!IsInMap(id))
	{
		return FALSE;
	}

	return mapItem[id] = data;
}


/******************************************************************************/
// 削除
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> BOOL Map<class ID, class DATA>::Delete(const ID& id)
{
	if(!IsInMap(id))
	{
		return FALSE;
	}

	map<ID, DATA>::iterator p = GetIterator(id);
	mapItem.erase(p);

	return TRUE;
}


/******************************************************************************/
// 全消去
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> BOOL Map<class ID, class DATA>::Clear()
{
	while(GetSize() > 0)
	{
		map<ID, DATA>::iterator p = mapItem.begin();
		mapItem.erase(p);
	}
	return TRUE;
}


/******************************************************************************/
// 要素取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> T Map<class ID, class DATA>::at(const ID& id)
{
	if(!IsInMap(id))
	{
		return 0;
	}

	return mapItem[id];
}


/******************************************************************************/
// コンスタント要素取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> const T Map<class ID, class DATA>::at(const ID& id) const
{
	if(!IsInMap(id))
	{
		return 0;
	}

	return mapItem[id];
}


/******************************************************************************/
//		付加動作
/******************************************************************************/
// 要素取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> T Map<class ID, class DATA>::operator[](UINT ui)
{
	return at(ui);
}


/******************************************************************************/
// コンスタント要素取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> const T Map<class ID, class DATA>::operator[](UINT ui) const
{
	return at(ui);
}


/******************************************************************************/
// 検索
//============================================================================//
// 概要：一番最初に引数と等しい物を返す。
// 補足：なし。
//============================================================================//

template <class ID, class DATA> UINT Map<class ID, class DATA>::Find(const T& t, UINT uiIndexStart) const
{
	UINT ui;
	for(ui = uiIndexStart; ui < GetSize(); ui++)
	{
		if(t == at(ui))
		{
			return ui;
		}
	}

	return -1;
}

#endif
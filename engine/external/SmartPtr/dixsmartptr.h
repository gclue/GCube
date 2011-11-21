// DixSmartPtr.h

// スマートポインタテンプレート  v2.24
//  Created by IKD (2010. 3. 18) 
//   http://marupeke296.com


// 2007. 2. 11
// スワップに対応

// 2007. 3. 1
// 同形コピーでコピーコンストラクタが呼ばれない不具合を修正
// 明示的アップキャスト及び暗黙的アップキャストでダブルポインタが代入されない不具合を修正

// 2007. 3. 6
// vectorに代入できないバグを修正（コピーコンストラクタのミスを修正）

// 2008. 5. 8
// 配列サポート

// 2008. 6. 30( v2.12 )
// 型キャスト代入の際に保護メンバであるm_isAryにアクセスできないコンパイルエラーを修正

// 2009. 4. 29( v2.20 )
// ウィークポインタ追加

// 2009. 5. 4( v2.21 )
// ウィークポインタにスマートポインタのアップキャスト代入コンストラクタを追加
// ダウンキャストメソッドのエラーを修正

// 2010. 1. 8( v.2.22 )
// 間接参照演算子及びアロー演算子にconst関数追加（constが付いた関数内で参照できないバグを回避）

// 2010. 1. 15( v2.23 )
// 型キャスト代入の際に保護メンバであるm_isAryにアクセスできないコンパイルエラーを修正
// ゲッターメソッド群にconst付記

// 2010. 3. 18( v2.24 )
// ウィークポインタのIsExistメソッドが参照カウントを見ていなかったバグを修正
// あるアドレスを確保していたスマートポインタを一度消した後、内部で同じアドレスが確保された場合に
// ウィークポインタにそのスマートポインタが代入されないバグを修正

// 2010, 3. 30( v2.25 )
// NULL比較演算子を正しく動くようにした


// ・ 登録されたオブジェクトポインタの使用者がいなくなったら自動的にdelete
// ・ 通常のポインタ操作演算子「*」「->」「[ ]」が使える。
// ・ 異なるオブジェクトポインタが登録された状態でアップキャストができる
// ・ DownCastメソッドによる安全なダウンキャストサポート
// ・ SwapPtrメソッドによるポインタ入れ替えのサポート
// ・ ウィークポインタサポート


#ifndef IKD_DIXSMARTPTR_H
#define IKD_DIXSMARTPTR_H

namespace Dix {

	// スマートポインタテンプレート
	template <class T>
	class sp
	{
	protected:
		template<class> friend class wp;

	private:
		unsigned int*	m_pRefCnt;		// 参照カウンタへのポインタ
		unsigned int*	m_pWeakCnt;		// ウィークカウンタへのポインタ
		T**				m_ppPtr;		// T型のオブジェクトのダブルポインタ
		static T*		m_NullPtr;		// NULLポインタ値
		bool				m_isAry;		// 配列か？


	private:
		/////////////////
		// 参照カウンタ増加
		/////
		void AddRef() {
			if ( m_pRefCnt != 0 )
				(*m_pRefCnt)++;
			if ( m_pWeakCnt != 0 )
				(*m_pWeakCnt)++;
		}

		/////////////////
		// 参照カウンタ減少
		/////
		void Release()
		{
			// 参照カウンタ減少
			if( m_pRefCnt != 0 && ( *m_pRefCnt == 0 || --(*m_pRefCnt) == 0 ) ) {

				if ( m_isAry )
					delete[] *m_ppPtr;	// 配列メモリ削除
				else
					delete *m_ppPtr;	// 単位メモリ削除

				delete m_ppPtr;
			}

			// ウィークカウンタチェック
			if ( m_pWeakCnt != 0 && ( *m_pWeakCnt == 0 || --(*m_pWeakCnt) == 0 ) ) {
				delete m_pWeakCnt;
				delete m_pRefCnt;
			}
		}


	public:
       ///////////////////////
	   // デフォルトコンストラクタ
	   /////
		explicit sp( T* src=0, int add=0 ) :
			m_pRefCnt	( 0 ),
			m_pWeakCnt	( 0 ),
			m_ppPtr		( 0 )
		{
			SetPtr( src, add );
		}

		//////////////////////////////////////////////
		// 配列フラグ付きデフォルトコンストラクタ
		/////
		sp(T* src, bool isAry, int add=0) :
			m_pRefCnt	( 0 ),
			m_pWeakCnt	( 0 ),
			m_ppPtr		( 0 )
		{
			SetPtr( src, isAry, add );
		}

		//////////////////////////////////////
		// コピーコンストラクタ（同型コピー）
		/////
		sp( const sp<T> &src ) :
			m_pRefCnt	( 0 ),
			m_pWeakCnt	( 0 ),
			m_ppPtr		( 0 )
		{
			// 相手がNULLポインタの場合は
			// 自分自身を空にする
			if ( src.m_pRefCnt == 0 ) {
				Clear();
			} else {
				// 相手のポインタをすべてコピー
				m_pRefCnt  = src.m_pRefCnt;
				m_pWeakCnt = src.m_pWeakCnt;
				m_ppPtr    = src.m_ppPtr;
				m_isAry    = src.IsAry();

				AddRef();
			}
		}

		//////////////////////////////////////
		// コピーコンストラクタ（暗黙的アップキャスト）
		/////
		template<class T2> sp(sp<T2> &src) :
			m_pRefCnt	( 0 ),
			m_pWeakCnt	( 0 ),
			m_ppPtr		( 0 )
		{
			// 相手がNULLポインタの場合は
			// 自分自身を空にする
			if ( src.GetRefPtr() == 0 ) {
				Clear();
			} else {
				m_pRefCnt  = src.GetRefPtr();
				m_pWeakCnt = src.GetWeakCntPtr();
				m_ppPtr    = (T**)src.GetPtrPtr();

				// 型チェックコピー
				*m_ppPtr = src.GetPtr();

				m_isAry = src.IsAry();

				AddRef();
			}
		}


		//////////////////////////////////////
		// コピーコンストラクタ（NULL代入代用）
		/////
		sp(const int nullval) :
			m_pRefCnt	( 0 ),
			m_pWeakCnt	( 0 ),
			m_ppPtr		( 0 ),
			m_isAry		( false )
		{
			// 空っぽにする
		}


	   ///////////////
	   // デストラクタ
	   /////
		 virtual ~sp()
		  {
			 Release();
		  }


	//■■■演算子のオーバーロード■■■■■■■■■■■


		/////////////////////////////////
		// =代入演算子（明示的コピー）
		/////
		sp<T>& operator =( const sp<T> &src )
		{
			// srcがNULLポインタの場合は
			// 空実装にする
			if ( src.m_pRefCnt == 0 ) {
				Clear();
			} else {
				if ( m_pRefCnt != 0 ) {
					// 同じオブジェクト同士のコピーは
					// 意味がないので行わない。
					if ( *src.m_ppPtr == *m_ppPtr )
						return (*this);
				}

				// 自分は他人になってしまうので
				// 参照カウンタを1つ減少
				Release();

				// 相手のポインタをコピー
				m_pRefCnt  = src.m_pRefCnt;
				m_pWeakCnt = src.m_pWeakCnt;
				m_ppPtr    = src.m_ppPtr;
				m_isAry    = src.IsAry();
				AddRef();
			}
			return (*this);
		}

		/////////////////////////////////
		// =代入演算子（明示的アップキャスト）
		/////
		template<class T2> sp<T>& operator =( sp<T2> &src )
		{
			// srcがNULLポインタの場合は
			// 空実装にする
			if ( src.GetRefPtr() == 0 ) {
				Clear();
			} else {
				if ( m_pRefCnt != 0 ) {
					// 同じオブジェクト同士のコピーは
					// 意味がないので行わない。
					if ( *src.GetPtrPtr() == *m_ppPtr )
						return (*this);
				}

				// 自分は他人になってしまうので
				// 参照カウンタを1つ減少
				Release();

				// 相手のポインタをコピー
				m_pRefCnt  = src.GetRefPtr();
				m_pWeakCnt = src.GetWeakCntPtr();
				m_ppPtr    = (T**)src.GetPtrPtr();
				m_isAry    = src.IsAry();

				// 型チェックコピー
				*m_ppPtr = src.GetPtr();

				AddRef();
			}

			return (*this);
		  }


		/////////////////////////////////
		// =代入演算子(NULL代入によるリセット）
		/////
		sp<T>& operator =( const int nullval )
		{
			// 自分は空な人になってしまうので
			// 参照カウンタを1つ減少
			Clear();
			return (*this);
		}
	   
		///////////////
		// *間接演算子
		/////
		T& operator *() {
			return **m_ppPtr;
		}
		T& operator *() const {
			return **m_ppPtr;
		}

		//////////////////////
		// ->メンバ選択演算子
		/////
		T* operator ->(){
			return *m_ppPtr;
		}
		T* operator ->() const {
			return *m_ppPtr;
		}

		/////////////////
		// ==比較演算子
		/////
		bool operator ==( sp<T>& val ) const {
			if( m_ppPtr != 0 && *m_ppPtr == val.GetPtr() )
				return true;
			if( m_ppPtr == 0 && val.GetPtr() == 0 )
				return true;
			return false;
		}

		bool operator ==( T *val ) const {
			if( m_ppPtr != 0 && *m_ppPtr == val )
				return true;
			return false;
		}

		bool operator ==( const int nul ) const {	// NULLとの比較専用
			if ( m_ppPtr == 0 || *m_ppPtr == 0 )
				return true;
			return false;
		}

		/////////////////
		// !=比較演算子
		/////
		bool operator !=( sp<T>& val ) const {
			if( m_ppPtr != 0 && *m_ppPtr != val.GetPtr() )
				return true;
			return false;
		}

		bool operator !=( T *val ) const {
			if( m_ppPtr != 0 && *m_ppPtr != val )
				return true;
			return false;
		}

		bool operator !=( const int nul ) const {	// NULLとの比較専用
			if ( m_ppPtr == 0 || *m_ppPtr == 0 )
				return false;
			return true;
		}

		///////////////////
		// 配列参照演算子
		/////
		T& operator[]( int elem ) const {
			return (*m_ppPtr)[ elem ];
		}

		///////////////////
		// ポインタ演算子
		/////
		T* operator+( int add ) const {
			return ( *m_ppPtr + add );
		}


	//■■■ メンバ関数 ■■■■■■■■■■■

	public:
		//////////////////////////
		// ポインタの明示的な登録
		/////
		void SetPtr( T* src = NULL, int add = 0 )
		{
			// 参照カウンタを減らした後に再初期化
			Release();

			// Srcが無効の場合はすべてをNULLにします
			if ( src == 0 ) {
				Clear();
			} else {
				// Srcが有効なので参照カウンタを増加させます
				m_pRefCnt = new unsigned int;
				*m_pRefCnt = add;
				m_pWeakCnt = new unsigned int;
				*m_pWeakCnt = add;
				m_ppPtr = new T*;
				*m_ppPtr = src;

				AddRef();       // 参照カウンタ増加
			}

			m_isAry = false;
		}

	   ////////////////////////////////////////////
	   // ポインタの明示的な登録（配列フラグ付き）
	   /////
		void SetPtr( T* src, bool isAry, int add = 0)
		{
			// 参照カウンタを減らした後に再初期化
			Release();
			// Srcが無効の場合はすべてをNULLにします
			if ( src == 0 ) {
				Clear();
			} else {
				// Srcが有効なので参照カウンタを増加させます
				m_pRefCnt = new unsigned int;
				*m_pRefCnt = add;
				m_pWeakCnt = new unsigned int;
				*m_pWeakCnt = add;
				m_ppPtr = new T*;
				*m_ppPtr = src;

				AddRef();       // 参照カウンタ増加
			}
			if ( src ) {
				m_isAry = isAry;
			}
	}

		/////////////////////
		// クリア
		/////
		void Clear() {
			Release();
			m_pRefCnt  = 0;
			m_pWeakCnt = 0;
			m_ppPtr    = 0;
			m_isAry    = 0;
		}

		/////////////////////
		// ポインタの貸し出し
		/////
		T* GetPtr() const { 
			if ( m_ppPtr == 0 )
				return 0;
			return *m_ppPtr;
		}

		T** GetPtrPtr() const {
			return m_ppPtr;
		}

	   /////////////////////
	   // 値取得
	   /////
		  T& At( int elem = 0 ) const {
			  return (*m_ppPtr)[ elem ];
		  }

		///////////////////////////////
		// 参照カウンタへのポインタを取得
		/////
		unsigned int* GetRefPtr() const { 
			return m_pRefCnt;
		}

		///////////////////////////////
		// 参照カウンタ数を取得
		/////
		unsigned int GetRefNum() const { 
			if ( m_pRefCnt == 0 ) 
				return 0;
			return *m_pRefCnt;
		}

		///////////////////////////////
		// ウィークポインタカウンタへのポインタを取得
		/////
		unsigned int* GetWeakCntPtr() const {
			return m_pWeakCnt;
		}

		///////////////////////////////
		// ウィークカウンタ数を取得
		/////
		unsigned int GetWeakNum() const {
			if ( m_pWeakCnt == 0 )
				return 0;
			return *m_pWeakCnt;
		}

		//////////////////////////
		// ダウンキャストコピー
		/////
		template <class T2> bool DownCast( sp<T2> &src )
		{
			if ( src.GetRefPtr() == 0 ) {
				return false;
			}

			// 引数のスマートポインタの持つポインタが、
			// 自分の登録しているポインタに
			// ダウンキャスト可能な場合はダウンキャストコピーを実行
			T* castPtr = dynamic_cast<T*>( src.GetPtr() );

			if( castPtr ){
				// ダウンキャスト成功
				// 自分は違う人になるので
				// 既存の参照カウンタを1つ減少
				Release();

				// 新しいポインタと参照カウンタを共有
				m_ppPtr    = (T**)src.GetPtrPtr();
				*m_ppPtr   = castPtr;
				m_pRefCnt  = src.GetRefPtr();
				m_pWeakCnt = src.GetWeakCntPtr();
				m_isAry    = src.IsAry();

				AddRef();
				return true;
			}

			return false;
		}

		///////////////////////
		// ポインタスワップ（交換）
		///////
		bool SwapPtr( sp<T> &src )
		{
			// 双方が有効ポインタである事を前提にします
			if ( src.GetRefPtr() == 0 || GetRefPtr() == 0 )
				return false;

			// ポインタの交換
			T* pTmp		 = src.GetPtr();
			*src.m_ppPtr = *m_ppPtr;
			*m_ppPtr	 = pTmp;

			// 配列フラグの交換
			bool tmp	 = src.IsAry();
			src.m_isAry  = m_isAry;
			m_isAry		 = tmp;

			return true;
		}

		///////////////////////
		// 配列？
		///////
		bool IsAry() const {
			return m_isAry;
		}
	};

	template <class T>
	T* sp<T>::m_NullPtr = 0;




	// ウィークポインタ
	template <class T>
	class wp
	{
	private:
		unsigned int*	m_pRefCnt;		// 参照カウンタへのポインタ
		unsigned int*	m_pWeakCnt;		// ウィークカウンタへのポインタ
		T**				m_ppPtr;		// オブジェクトへのダブルポインタ
		static T*		m_NullPtr;		// NULLポインタ値
		bool			m_isAry;		// 配列か？


	private:
		///////////////////////
		// 参照カウンタ増加
		/////
		unsigned int AddRef() {
			if ( m_pWeakCnt == 0 )
				return 0;
			return ++(*m_pWeakCnt);
		}

		///////////////////////
		// 参照カウンタ減少
		/////
		unsigned int Release() {
			if ( m_pWeakCnt != 0 ) {
				if ( --(*m_pWeakCnt) == 0 ) {
					// ウィークカウンタ削除
					delete m_pWeakCnt;
					delete m_pRefCnt;
					return 0;
				}
				return *m_pWeakCnt;
			}
			return 0;
		}

	public:
		///////////////////////
		// コンストラクタ
		/////
		wp() :
			m_ppPtr     ( 0 ),
			m_pRefCnt   ( 0 ),
			m_pWeakCnt  ( 0 ),
			m_isAry		( false )
		{
		}

		wp( sp<T>& src ) {
			m_ppPtr    = src.GetPtrPtr();
			m_pRefCnt  = src.GetRefPtr();
			m_pWeakCnt = src.GetWeakCntPtr();
			m_isAry = src.IsAry();
			
			AddRef();
		}

		//////////////////////////////////////
		// コピーコンストラクタ（同型コピー）
		/////
		wp( const wp<T> &src )
		{
			// 相手のポインタをすべてコピー
			m_pRefCnt  = src.m_pRefCnt;		// 参照カウンタポインタ
			m_pWeakCnt = src.m_pWeakCnt;	// ウィークカウンタポインタ
			m_ppPtr    = src.m_ppPtr;       // T型ダブルポインタ
			m_isAry    = src.IsAry();

			// 自分自身の参照カウンタを増加
			AddRef();
		}

		//////////////////////////////////////
		// コピーコンストラクタ（暗黙的アップキャスト）
		/////
		template<class T2> wp( wp<T2> &src )
		{
			// 相手のポインタをコピー
			m_pRefCnt  = src.GetRefPtr();
			m_pWeakCnt = src.GetWeakCntPtr();
			m_ppPtr	   = (T**)src.GetPtrPtr();

			// 型チェックコピー
			if ( m_ppPtr )
				*m_ppPtr = src.GetPtr();

			m_isAry = src.IsAry();

			// 自分自身の参照カウンタを増加
			AddRef();
		}

		//////////////////////////////////////
		// コピーコンストラクタ（スマートポインタ暗黙的アップキャスト）
		/////
		template<class T2> wp( sp<T2> &src ) :
			m_ppPtr     ( 0 ),
			m_pRefCnt   ( 0 ),
			m_pWeakCnt  ( 0 ),
			m_isAry		( false )
		{
			// 相手がNULLポインタだった場合は
			// 無条件で自分もNULLポインタになる
			if ( src.GetPtr() == 0 ) {
				Clear();
			} else {
				T* p = (T*)dynamic_cast<T*>( src.GetPtr() );
				if ( p != 0 ) {
					// 相手のポインタをコピー
					m_pRefCnt  = src.GetRefPtr();
					m_pWeakCnt = src.GetWeakCntPtr();
					m_ppPtr    = (T**)src.GetPtrPtr();

					m_isAry = src.IsAry();

					// 新しい自分自身の参照カウンタを増加
					AddRef();
				}
			}
		}

		//////////////////////////////////////
		// コピーコンストラクタ（NULL代入代用）
		/////
		wp( const int nullval ) :
			m_ppPtr     ( 0 ),
			m_pRefCnt   ( 0 ),
			m_pWeakCnt  ( 0 ),
			m_isAry		( false )
		{
		}

		///////////////
		// デストラクタ
		/////
		virtual ~wp()
		{
			Release();
		}


	//■■■演算子のオーバーロード■■■■■■■■■■■


		/////////////////////////////////
		// =代入演算子（明示的コピー）
		/////
		wp<T>& operator =( const wp<T> &src )
		{
			// 自分が空でなければチェックへ
			if ( m_pWeakCnt != 0 ) {
				// スマートポインタがオブジェクトを解放していない
				// 場合はm_ppPtrが有効なのでチェック
				if ( src.m_pRefCnt != 0 && *src.m_pRefCnt != 0 ) {
					// 自分自身への代入は不正で意味が無いので
					// 行わない。
					if ( src.m_ppPtr != 0 && *src.m_ppPtr == *m_ppPtr )
						return (*this);
				}

				// 自分は他人になってしまうので
				// 参照カウンタを1つ減少
				Release();
			}

			// 相手のポインタをコピー
			m_pRefCnt  = src.m_pRefCnt;
			m_pWeakCnt = src.m_pWeakCnt;
			m_ppPtr    = src.m_ppPtr;

			m_isAry    = src.IsAry();

			// 新しい自分自身の参照カウンタを増加
			AddRef();

			return (*this);
		}

		/////////////////////////////////
		// =代入演算子（明示的コピー）
		/////
		wp<T>& operator =( sp<T> &src )
		{
			// 自分が空でなければチェックへ
			if ( m_pWeakCnt != 0 ) {
				if ( src.GetRefPtr() != 0 && *src.GetRefPtr() != 0 ) {
					// 自分自身への代入は不正で意味が無いので行わない。
					// ただし、自分が参照しているスマートポインタの
					// 参照カウンタが0の場合は代入を許可する。
					if ( GetRefNum() != 0 && src.GetPtrPtr() != 0 && *src.GetPtrPtr() == *m_ppPtr )
						return (*this);
				}

				// 自分は他人になってしまうので
				// 参照カウンタを1つ減少
				Release();
			}

			// 相手のポインタをコピー
			m_pRefCnt  = src.GetRefPtr();
			m_pWeakCnt = src.GetWeakCntPtr();
			m_ppPtr    = src.GetPtrPtr();

			m_isAry = src.IsAry();

			// 新しい自分自身の参照カウンタを増加
			AddRef();

			return (*this);
		}


		/////////////////////////////////
		// =代入演算子（明示的キャスト）
		/////
		template<class T2> wp<T>& operator =( sp<T2> &src ) {

			// 相手がNULLポインタだった場合は
			// 無条件で自分もNULLポインタになる
			if ( src.GetPtr() == 0 ) {
				Clear();
				return (*this);
			}

			T* p = (T*)dynamic_cast<T*>( src.GetPtr() );
			if ( p == 0 ) {
				return (*this);
			} else {
				// 自分が空でなければチェックへ
				if ( m_pWeakCnt != 0 ) {
					// スマートポインタがオブジェクトを解放していない
					// 場合はm_ppPtrが有効なのでチェック
					if ( src.GetRefPtr() != 0 && *src.GetRefPtr() != 0 ) {
						// 自分自身への代入は不正で意味が無いので
						// 行わない。
						if ( src.GetPtrPtr() != 0 && *src.GetPtrPtr() == *m_ppPtr )
							return (*this);
					}

					// 自分は他人になってしまうので
					// 参照カウンタを1つ減少
					Release();
				}

				// 相手のポインタをコピー
				m_pRefCnt  = src.GetRefPtr();
				m_pWeakCnt = src.GetWeakCntPtr();
				m_ppPtr    = (T**)src.GetPtrPtr();

				m_isAry = src.IsAry();

				// 新しい自分自身の参照カウンタを増加
				AddRef();
			}

			return (*this);
		}

		/////////////////////////////////
		// =代入演算子（明示的アップキャスト）
		/////
		template<class T2> wp<T>& operator =( wp<T2> &src )
		{
			// 自分が空でなければチェックへ
			if ( m_pWeakCnt != 0 ) {
				// スマートポインタがオブジェクトを解放していない
				// 場合はm_ppPtrが有効なのでチェック
				if ( src.m_pRefCnt != 0 && *src.m_pRefCnt != 0 ) {
					// 自分自身への代入は不正で意味が無いので
					// 行わない。
					if ( src.m_ppPtr != 0 && *src.m_ppPtr == *m_ppPtr )
						return (*this);
				}

				// 自分は他人になってしまうので
				// 参照カウンタを1つ減少
				Release();
			}

			// 相手のポインタをコピー
			m_pRefCnt = src.GetRefPtr();
			m_pWeakCnt = src.GetWeakCntPtr();
			m_ppPtr = (T**)src.GetPtrPtr();

			// 型チェックコピー
			if ( m_ppPtr )
				*m_ppPtr = src.GetPtr();

			m_isAry = src.IsAry();

			// 新しい自分自身の参照カウンタを増加
			AddRef();

			return (*this);
		}

		/////////////////
		// ==比較演算子
		/////
		bool operator ==( sp<T>& val ) const {
			if( m_ppPtr != 0 && *m_ppPtr == val.GetPtr() )
				return true;
			if( m_ppPtr == 0 && val.GetPtr() == 0 )
				return true;
			return false;
		}

		bool operator ==( wp<T>& val ) const {
			if( m_ppPtr != 0 && *m_ppPtr == val.GetPtr() )
				return true;
			if( m_ppPtr == 0 && val.GetPtr() == 0 )
				return true;
			return false;
		}

		bool operator ==( const int nul ) const {	// NULLとの比較専用
			if ( m_ppPtr == 0 || *m_ppPtr == 0 )
				return true;
			return false;
		}

		/////////////////
		// !=比較演算子
		/////
		bool operator !=( sp<T>& val ) const {
			if( m_ppPtr != 0 && *m_ppPtr != val.GetPtr() )
				return true;
			return false;
		}

		bool operator !=( wp<T>& val ) const {
			if( m_ppPtr != 0 && *m_ppPtr != val.GetPtr() )
				return true;
			return false;
		}

		bool operator !=( const int nul ) const {	// NULLとの比較専用
			if ( m_ppPtr == 0 || *m_ppPtr == 0 )
				return false;
			return true;
		}

		///////////////////
		// 配列参照演算子
		/////
		T& operator[]( int elem ) const {
			return (*m_ppPtr)[ elem ];
		}

		///////////////////
		// ポインタ演算子
		/////
		T* operator+( int add ) const {
			return ( *m_ppPtr + add );
		}

		///////////////
		// *間接演算子
		/////
		T& operator *() const {
			return **m_ppPtr;
		}

		//////////////////////
		// ->メンバ選択演算子
		/////
		T* operator ->() const {
			return m_ppPtr ? *m_ppPtr : 0;
		}


	//■■■ メンバ関数 ■■■■■■■■■■■

	public:
		/////////////////////
		// スマートポインタ作成
		/////
		sp<T> GetSmartPtr() {

			sp<T> tmp;
			if ( m_pRefCnt == 0 || *m_pRefCnt == 0 ) {
				return tmp;
			}

			tmp.m_ppPtr = GetPtrPtr();
			tmp.m_pRefCnt = m_pRefCnt;
			tmp.m_pWeakCnt = m_pWeakCnt;
			tmp.m_isAry = m_isAry;

			tmp.AddRef();

			return tmp;
		}

		/////////////////////
		// クリア
		/////
		void Clear() {
			Release();
			m_pRefCnt = 0;
			m_pWeakCnt = 0;
			m_ppPtr = 0;
			m_NullPtr = 0;
			m_isAry = false;
		}

		/////////////////////
		// ポインタの貸し出し
		/////
		T* GetPtr() const {
			// オブジェクトがすでに削除されている場合はNULLを返す
			if ( m_pRefCnt == 0 || *m_pRefCnt == 0 )
				return 0;
			return *m_ppPtr;
		}

		T** GetPtrPtr() const {
			return m_ppPtr;
		}

		/////////////////////
		// 値取得
		/////
		T& At( int elem = 0 ) const {
			return (*m_ppPtr)[ elem ];
		}

		///////////////////////////////
		// 参照カウンタへのポインタを取得
		/////
		unsigned int* GetRefPtr() const {
			return m_pRefCnt;
		}

		///////////////////////////////
		// 参照カウンタ数を取得
		/////
		unsigned int GetRefNum() const {
			return IsExist() ? *m_pRefCnt : 0;
		}

		///////////////////////////////
		// ウィークカウンタへのポインタを取得
		/////
		unsigned int* GetWeakCntPtr() const {
			return m_pWeakCnt;
		}

		///////////////////////////////
		// ウィークカウンタ数を取得
		/////
		unsigned int GetWeakNum() const {
			if ( m_pWeakCnt == 0 )
				return 0;
			return *m_pWeakCnt;
		}

	   //////////////////////////
	   // spダウンキャストコピー
	   /////
		template <class T2> bool DownCast( sp<T2> &src )
		{
			// 相手がNULLポインタだった場合は
			// 無条件で自分もNULLポインタになる
			if ( src.GetPtr() == 0 ) {
				Clear();
				return true;
			}

			// 引数のスマートポインタの持つポインタが、
			// 自分の登録しているポインタに
			// ダウンキャスト可能な場合はダウンキャストコピーを実行
			T* castPtr = dynamic_cast<T*>( src.GetPtr() );

			if( castPtr ) {

				// ダウンキャスト成功
				// 自分は違う人になるので
				// 既存のウィークカウンタを1つ減少
				Release();

				// 新しいポインタと参照カウンタを共有
				m_ppPtr = (T**)src.GetPtrPtr();
				if ( m_ppPtr )
					*m_ppPtr = castPtr;
				m_pRefCnt = src.GetRefPtr();
				m_pWeakCnt = src.GetWeakCntPtr();

				m_isAry = src.IsAry();

				// 参照カウンタ増加
				AddRef();

				return true;
			}

			return false;
		}

	   //////////////////////////
	   // ダウンキャストコピー
	   /////
		template <class T2> bool DownCast( wp<T2> &src )
		{
			// 相手がNULLポインタだった場合は
			// 無条件で自分もNULLポインタになる
			if ( src.GetPtr() == 0 ) {
				Clear();
				return true;
			}

			// 引数のウィークポインタの持つポインタが、
			// 自分の登録しているポインタに
			// ダウンキャスト可能な場合はダウンキャストコピーを実行
			T* castPtr = dynamic_cast<T*>( src.GetPtr() );

			if( castPtr ) {

				// ダウンキャスト成功
				// 自分は違う人になるので
				// 既存のウィークカウンタを1つ減少
				Release();

				// 新しいポインタと参照カウンタを共有
				m_ppPtr = (T**)src.GetPtrPtr();
				*m_ppPtr = castPtr;
				m_pRefCnt = src.GetRefPtr();
				m_pWeakCnt = src.GetWeakCntPtr();

				m_isAry = src.IsAry();

				// 参照カウンタ増加
				AddRef();

				return true;
			}

			return false;
		}

		///////////////////////
		// ポインタスワップ（交換）
		///////
		bool SwapPtr( wp<T> &src )
		{
			// 双方が有効ポインタである事を前提にします
			if ( src.IsExist() == 0 || IsExist() == 0 )
				return false;

			// ポインタの交換
			T* pTmp = src.GetPtr();
			*src.m_ppPtr = *m_ppPtr;
			*m_ppPtr = pTmp;

			// 配列フラグの交換
			bool tmp = src.IsAry();
			src.m_isAry = m_isAry;
			m_isAry = tmp;

			return true;
		}

		///////////////////////
		// 実体が存在してる？
		///////
		bool IsExist() const {
			return (!m_pRefCnt || (*m_pRefCnt == 0)) ? false : true;
		}

		///////////////////////
		// 配列？
		///////
		bool IsAry() const {
			return m_isAry;
		}
	};


	template <class T>
	T* wp<T>::m_NullPtr = 0;

} // end namespace Dix


#endif // !defined(AFX_SMARTPTR_H__4D5E5A30_6FF3_4C18_ACEC_19EA17209FC1__INCLUDED_)

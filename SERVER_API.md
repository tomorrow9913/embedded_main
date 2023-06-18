# Server API Docs
2023-06-18

## HTTPie (HTTP Client)
- [Website](https://httpie.io/)

## 🔒 상품 (Item)

### 상품 등록
```bash
http -a admin:admin post :3000/item id=${상품 고유번호} price:=${상품 가격} count:=${상품 개수}
```

### 상품 조회
```bash
http -a admin:admin get :3000/item/${상품 고유번호}
```

## 상품 조회(전체)
```bash
http -a admin:admin get :3000/item
```

### 상품 정보 수정 (일부 항목 수정)
```bash
http -a admin:admin patch :3000/item id=${상품 고유번호} price:=${상품 가격} count:=${상품 개수}
```

### 상품 정보 수정 (전체 항목 수정)
```bash
http -a admin:admin put :3000/item id=${상품 고유번호} price:=${상품 가격} count:=${상품 개수}
```

### 상품 삭제
```bash
http -a admin:admin delete :3000/item/${상품 고유번호}
```

---
## 🔒 사용자 (User)

### 사용자 등록
```bash
http -a admin:admin post :3000/user id=${사용자 카드번호} balance:=${잔액}
```

### 사용자 조회
```bash
http -a admin:admin get :3000/user/${사용자 카드번호}
```

### 사용자 정보 수정 (일부 수정)
```bash
http -a admin:admin patch :3000/user id=${사용자 카드번호} balance:=${잔액}
```

### 사용자 정보 수정 (전체 수정)
```bash
http -a admin:admin put :3000/user id=${사용자 카드번호} balance:=${잔액}
```

### 사용자 삭제
```bash
http -a admin:admin delete :3000/user/${사용자 카드번호}
```

---
## 🌏 구매 (Purchase)

### 장바구니 생성
```bash
http --session=./session.json post :3000/purchase
```

### 장바구니 보관 중인 상품 조회
```bash
http --session=./session.json get :3000/purchase
```

### 장바구니 보관 중인 상품 조회 (다른 사용자)
```bash
http get :3000/purchase/${세션ID}
```

### 장바구니 삭제
```bash
http --session=./session.json delete :3000/purchase
```

### 장바구니 상품 추가
```bash
http --session=./session.json post :3000/purchase/item ItemID=${상품 고유번호} count:=${상품 개수}
```

### 장바구니 상품 삭제
```bash
http --session=./session.json delete :3000/purchase/item id=${상품 고유번호} count:={삭제할 상품 개수}
```

### 구입 진행
```bash
http --session=./session.json get :3000/purchase/sign/${사용자 id}
```

---
## 🌏 장바구니 정보 공유 (Sync)

### 장바구니 정보 저장
```bash
http post :3000/sync/${고유번호}
```

### 장바구니 정보 동기화
```bash
http get :3000/sync/${고유번호}
```

---
## 🔒 판매 기록 (Log)

### 판매 기록 조회
```bash
http -a admin:admin get :3000/log
```

### 판매 기록 조회 (id)
```bash
http -a admin:admin get :3000/log/${판매 기록 ID}
```

### 판매 기록 조회 (사용자)
```bash
http -a admin:admin get :3000/log/user/${사용자 카드번호}
```

---
## 🔒 판매자 정보 (Info)
- Key, Value 형태의 정보 저장소 (기타 정보 저장)

### 판매자 정보 등록
```bash
http -a admin:admin post :3000/info id=${제목} value=${내용}
```

### 판매자 정보 조회
```bash
$ http -a admin:admin get :3000/info/${제목}
```

### 판매자 정보 수정 (일부 수정)
```bash
http -a admin:admin patch :3000/info id=${제목} value=${내용}
```

### 판매자 정보 수정 (전체 수정)
```bash
http -a admin:admin put :3000/info id=${제목} value=${내용}
```


### 판매자 정보 삭제
```bash
http -a admin:admin delete :3000/info/${제목}
```
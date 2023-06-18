"Arduino UNO Code" 

##  서버 API 명령어
상품 기본 데이터 <br>
```text
1. 보리차    8808024031923    2000   20
2. 치약     8801051037098    1500   15
3. 피로회복제 8806417027416    4000    5
4. 지퍼팩    8801328501772    500    20
5. 물티슈    8809243429225    1200   10
```


상품 등록
```
http -a admin:admin post :3000/item id=8808024031923 name=water price:=2000 count:=20
http -a admin:admin post :3000/item id=8801051037098 name=Tpaste price:=1500 count:=15
http -a admin:admin post :3000/item id=8806417027416 name=tonic price:=4000 count:=5
http -a admin:admin post :3000/item id=8801328501772 name=pack price:=500 count:=20
http -a admin:admin post :3000/item id=8809243429225 name=wtissue price:=1200 count:=10
```

상품 개별 조회
```
http -a admin:admin get :3000/item/8808024031923 --pretty=none --print=b
http -a admin:admin get :3000/item/8801051037098 --pretty=none --print=b
http -a admin:admin get :3000/item/8806417027416 --pretty=none --print=b
http -a admin:admin get :3000/item/8801328501772 --pretty=none --print=b
http -a admin:admin get :3000/item/8809243429225 --pretty=none --print=b
```

상품 전체 조회
```
http -a admin:admin get :3000/item --pretty=none --print=b
```

터미널 생상 코드 제거 옵션
```
--pretty=none --print=b
```

### Template (템플릿)
- template`는 다양한 타입을 범용적으로 사용하기 위해 사용한다. 
```cpp
template<typename T>
T add_value(T t1, T t2)
{
	return t1 + t2;
}
```
- 이는 형식 추론을 통해 가능한데 `template`를 컴파일 할 때 사용한 타입에 대해 추론을 하며, 이를 오브젝트 파일로 만들어서 메모리에 등록한다. 




### Template 특수화
- `template`는 다양한 타입을 범용적으로 사용하기 위해 제작하나 그 중에서도 일부 타입에 대해서 다른 알고리즘을 적용하고자 할 경우 사용한다.

```cpp
template<typename T>
T add_value(T t1, T t2)
{
	return t1 + t2;
}

// 특수화
template<>
double add_alue(double t1, double t2)
{
	return (int)t1 + (int)t2;
}
```

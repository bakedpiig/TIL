## 지역변수를 선언할 때는 var를 사용하는 것이 낫다
---
C#에는 var 키워드가 있어서 지역변수의 타입을 암시할 수 있다. 또한 메서드의 반환 타입에 대해 오류를 방지할 수도 있다.

var를 과도하게 사용했을 때 가독성을 해칠 수도 있는 경우가 있지만 많지는 않다. 어려운 버그를 만들 경우도 있기는 하다. 하지만 그런 경우가 있는 경우는 코드를 잘 짜지 못했을 때 일어나는 것이다.

가령 메서드의 이름을 잘못 지어서 반환 타입을 이름만으로는 알 수 없는 경우가 있다. 이때 반환값을 받는 변수를 var로 설정을 하면 코드의 해석이 더 어려워진다. 이는 변수 이름을 타입 유추가 가능하게끔 지어놓으면 해결된다.

특별히 알아야 할 점은 숫자 타입은 var로 선언하는 것보다 명시적으로 선언하는 경우가 편하다. 숫자 타입 간에는 묵시적으로 변환 연산이 자주 일어나기 때문이다. 따라서 프로그래머가 원하지 않는 타입으로 선언되는 경우가 있다.

특히 LINQ 구문에서는 var가 왜 필요한지 알 수 있다. LINQ 쿼리의 반환 값은 IQueryable\<T>이지만, 예제와 같이 IEnumerable\<T>로 받아버린다면 IQueryable의 장점이 사라지게 된다.

var를 너무 자주 사용하면 가독성이 떨어지기도 한다. 따라서 지역변수의 타입을 명확히 유추할 수 없고 모호함을 부를 수 있다면 명시적으로 선언하는 것이 좋다.
## 캐스트보다는 is, as가 좋다

as 연산자는 C#에서 지원하는 형변환 수행 연산자이고, is는 형변환이 가능한지 확인할 수 있는 연산자이다.

그런데 원래 캐스트 연산자가 있는데 굳이 as를 써야할까? 그 이유는 as를 사용하면 더 안전하고 런타임에서도 효율적이기 때문이다.

일반 캐스트 연산자를 사용할 경우에는 예외처리 코드와 null 확인 코드가 필요하지만, as 연산자를 썼을 때는 null 확인도 동시에 가능하기 때문에 코드를 읽을 때에도 간단하다.

알아둬야 할 점은, as와 is 연산자는 사용자 정의 형변환을 다루지 않는다는 것이다. 따라서 사용자 정의 형변환을 as 연산자를 이용하면 null 값을 반환한다.

다만 형변환하려는 타입이 값 타입일 경우 as 연산자를 사용할 수 없다. 이 경우에는 nullable 타입으로 형변환을 수행하도록 한다.

그러면 foreach 루프를 사용할 때는 어떤 방법을 사용하는 것이 좋을지 알아보자. foreach 루프는 제네릭 타입이 아닌 IEnumerable 인터페이스를 이용한다. 그래서 다음 코드들은 비슷한 방식으로 돌아간다.

~~~c#
public void UseCollection(IEnumerable theCollection) {
    foreach(MyType t in theCollection)
        t.DoStuff();
}
~~~

~~~c#
public void UseCollection(IEnumerable theCollection) {
    IEnumerator it = theCollection.GetEnumerator();
    while(it.MoveNext()) {
        MyType t = (MyType)it.Current;
        t.DoStuff();
    }
}
~~~

it.Current는 System.Object 타입을 반환한다. 하지만 Object 타입은 사용자 정의 형변환이 없기 때문에 만약 사용자 정의 형변환이 있는 타입을 담은 컬렉션을 매개변수로 전달할 수 없다. 다만 만약 MyType을 상속한 타입의 컬렉션이라면 가능하다.
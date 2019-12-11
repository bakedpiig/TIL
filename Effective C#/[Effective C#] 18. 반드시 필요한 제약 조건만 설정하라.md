### 타입 매개변수에 대한 제약 조건
---
타입 매개변수에 너무 많은 제약 조건을 설정하면 이를 만족시키기 위해 과도한 추가 작업을 수행해야 할 수도 있다. 그렇다고 제약 조건을 설정하지 않으면 런타임에 더 많은 검사를 수행해야 한다. 따라서 필요한 만큼의 제약 조건을 찾는 것이 중요하다.

제약 조건 대신 형변환이나 런타임에 테스트를 수행하도록 할 수도 있다. 다만 오류가 런타임에 발생한다는 점과 런타임 시간이 길어진다는 점이 단점이다.
~~~c#
public static bool AreEqual<T>(T left, T right) {
    if(left == null)
        return right == null;

    if(left is IComparable<T>) {
        IComparable<T> lval = left as IComparable<T>;
        if(right is IComparable<T>)
            return lval.CompareTo(right) == 0;
        else
            throw new ArgumentException("Type does not implement IComparable<T>",nameof(right));
    }
    else {
            throw new ArgumentException("Type does not implement IComparable<T>",nameof(left));
    }
}
~~~

제약 조건을 설정한다면 이렇게 변한다.
~~~c#
public static bool AreEqual<T>(T left, T right)
    where T : IComparable<T> => left.CompareTo(right) == 0;
~~~

그렇다고 너무 많은 제약 조건을 설정하는 것 또한 좋지 않다. 부담이 될 수 있기 때문이다.

---
### 제약 조건을 최소화하는 방법
---
명확한 기능만 제약 조건으로 명시해야 한다.

만약 제약 조건으로 설정한 클래스에 재정의된 메서드가 기존 메서드보다 성능이 저하되거나 한다면 제약 조건을 설정해 주지 않아도 된다. 일반적으로 개선된 메서드를 사용하려 시도하되 그것이 불가능하다면 한 단계 낮은 수준의 메서드를 호출한다.
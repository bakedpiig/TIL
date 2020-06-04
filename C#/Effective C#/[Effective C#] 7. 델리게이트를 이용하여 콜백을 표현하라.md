델리게이트를 이용하면 타입 안정적인 콜백을 정의할 수 있다. 델리게이트가 좋은 것은 여러 클래스가 상호 통신을 수행해야 할 때 결합도를 낮출 수 있다는 것이다.

C#에서는 자주 사용되는 델리게이트를 다음과 같이 정의해 두었다.

* Predicate\<T>: 조건 검사
* Func\<>: 여러 개의 매개변수를 받아 마지막 받은 타입으로 반환
* Action\<>: 여러 개의 매개변수 받아 실행(반환 없음)

델리게이트의 멀티캐스트가 가능하다. 하지만 이는 두 가지 주의할 점이 있다.

1. 예외에 안전하지 않다.
1. 마지막으로 호출된 대상 함수의 반환값이 델리게이트의 반환값으로 간주된다.

함수 호출 중에 예외가 발생하면 델리게이트는 중단되며 이 전의 메서드의 반환값이 델리게이트의 반환값이 된다.

~~~c#
public void LengthyOperation(Func<bool> pred) {
    foreach(ComplicatedClass cl in container) {
        cl.DoLengthyOperation();
        if(pred()==false) {
            return;
        }
    }
}
~~~

이 메서드를 멀티캐스트 델리게이트를 인자로 건넬 경우 문제가 발생한다. 예제를 써 본다.

~~~c#
Func<bool> cp = () => CheckWithUser();
cp += () => CheckWithSystem();
c.LengthyOperation(cp);
~~~

위와 같은 코드의 경우, CheckWithUser()의 반환값은 무시가 된다.

이 문제를 해결하려면 델리게이트에 포함된 호출 대상 콜백 함수를 직접 다뤄야 한다. 다음과 같이 말이다.
~~~c#
public void LengthyOperation2(Func<bool> pred) {
    bool bContinue = true;
    foreach(ComplicatedClass cl in container) {
        cl.DoLengthyOperation();
        foreach(Func<bool> pr in pred.GetInvocationList())
            bContinue &= pr();
        
        if(!bContinue) return;
    }
}
~~~
Dispose 패턴은 .NET Framework 내부에서 비관리 리소스를 정리하는 표준화된 패턴이다. 이 패턴은 GC와 연계되어 동작하며 불가피한 경우에만 finalizer를 호출하도록 하여 성능에 미치는 부정적 영향을 최소화한다. 

.NET에서는 비관리 리소스를 *System.Runtime.Interop.SafeHandle*을 상속한 파생 클래스로 표현하는데 이 또한 Dispose 패턴이 구현되어 있다.

상속 계통 최상위의 베이스 클래스는 다음과 같은 작업을 수행해야 한다.
* 리소스 정리를 위해 *IDisposable* 인터페이스를 구현한다.
* 멤버 필드에 비관리 리소스가 있을 때 finalizer를 추가해야 한다.
* Dispose와 finializer는 실제 리소스 정리 작업을 수행하는 다른 가상 메서드에 작업을 위임하도록 작성돼야 한다.

파생 클래스는 다음 작업을 수행해야 한다.
* 고유의 리소스 정리가 필요하다면 베이스 클래스의 가상 메서드를 재정의한다.
* 멤버 필드에 비관리 리소스가 있을 때 finalizer를 추가해야 한다.
* 베이스 클래스에서 정의하고 있는 가상 함수를 반드시 재호출해야 한다.

*IDisposable*을 구현한다는 것은 사용자와 .NET 런타임에게 적시에 리소스를 정리할 수 있는 방법이 있다는 것을 알려주기 위한 표준화된 방법이다.

~~~c#
public interface IDisposable {
    void Dispose();
}
~~~

*Dispose*는 다음 작업을 수행해야 한다.
1. 모든 비관리 리소스를 정리한다.
1. 모든 관리 리소스를 정리한다.
1. 객체가 이미 정리되었음을 나타내기 위한 상태 플래그를 설정한다. 이미 설정되어 있으면 ObjectDisposed 예외를 발생시킨다.
1. finalizer 호출을 회피하기 위해 GC.SuppressFinalize(this)를 호출한다.

다만, 이런 방식으로 파생 클래스를 정의한다면 베이스 클래스와 파생 클래스가 각각 정리하는 방식이 꼬인다. 이를 위해서 가상 헬퍼 함수를 작성한다. 이 메서드는 파생 클래스에게 리소스를 정리할 기회를 주는 역할을 한다.
~~~c#
protected virtual void Dispose(bool isDisposing);
~~~

이 메서드의 규칙은 다음과 같다.
* 코드의 마지막에는 반드시 베이스 클래스의 Dispose를 호출한다.
* 관리 리소스와 비관리 리소스 모두를 제거하려면 true를, 관리 리소스만 제거하려면 false를 전달한다.

~~~c#
public class MyResourceHog: IDisposable {
    private bool alreadyDisposed = false;

    public void Dispose() {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool isDisposing) {
        if(alreadyDisposed) return;

        if(isDisposing) {
            //관리 리소스 정리
        }

        //비관리 리소스 정리

        alreadyDisposed = true;
    }

    public void ExampleMethod() {
        if(alreadyDisposed) {
            throw new ObjectDisposedException("MyResourceHog",
            "Called Example Method on Disposed object");
        }
    }
}
~~~

파생 클래스에서도 추가적인 리소스 정리가 필요하다면 다음과 같이 작성한다.

~~~c#
public class DerivedResourceHog: MyResourceHog {
    private bool disposed = false;
    
    protected override void Dispose(bool isDisposing) {
        if(disposed) return;

        if(isDisposing) {
            //관리 리소스 정리
        }

        //비관리 리소스 정리
        
        //베이스 클래스는 GC.SuppressFinalize()를 호출
        base.Dispose(isDisposing);

        disposed = true;
    }
}
~~~

Dispose 메서드는 호출될 때마다 동일하게 동작되도록 구현해야 한다.

---
### 왜 finalizer를 호출하면 성능이 좋지 않을까?
---
GC가 수행될 때 finalizer가 있는 객체는 finalizer 큐에 삽입되며 finalizer 스레드에서 순차적으로 호출한다. 그 말인 즉슨, 자동으로 한 세대 높아진다는 것이다. 메모리에 더 오래 살아남는다.

---

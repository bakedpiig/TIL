제네릭 클래스에서 타입 매개변수가 *IDisposable*을 구현하고 있다는 것은 리소스 정리에 관해서 특별히 해야 할 작업이 있다는 것이다.

이를 대비하기 위해서 using문을 사용하여 리소스를 관리한다. 다음은 어떻게 사용하는지 보여주는 예시이다.
~~~c#
public void GetThingsDone() {
    T driver = new T();
    using (driver as IDisposable) {
        driver.DoWork();
    }
}
~~~

정말 간단하게도 이것이 전부다. using문이 끝나면 driver가 null이 아니라면 Dispose()를 호출한다.

타입 매개변수로 전달받은 타입으로 멤버 변수를 선언했다면 Dispose()에 추가적인 해제 작업이 필요하다. T에 IDisposable이 구현되지 않을 경우를 대비해서 ?. 연산자를 이용할 수 있다.

~~~c#
public sealed class EngineDriver: IDisposable where T: IEngine, new() {
    private Lazy<T> driver = new Lazy<T>(() => new T());

    public void GetThingsDone() => driver.Value.DoWork();

    public void Dispose() {
        if(driver.IsValueCreated) {
            var resource = driver.Value as IDisposable;
            resource?.Dispose();
        }
    }
}
~~~
이벤트 호출에 있어서 문제점은 이벤트 핸들러가 null일 때이다. 이벤트 핸들러가 null인지 판단하고 바로 실행할 수 있는 null 조건 연산자(?.)가 있어서 이를 통해 해결할 수 있다.

이를 이용한 예시이다.
~~~c#
public class EventSource {
    private EventHandler<int> Updated;

    public void RaiseUpdates() {
        counter++;
        Updated?.Invoke(this, counter);
    }

    private int counter;
}
~~~

여기서 Invoke 메서드는 모든 델리게이트와 이벤트에 대하여 타입 안정적 형태로 생성해주는 메서드이고, 이벤트를 직접 발생시키는 코드와 동일한 성능을 가진다.
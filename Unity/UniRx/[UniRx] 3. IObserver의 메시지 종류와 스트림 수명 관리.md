# IObserver의 메시지 종류와 스트림 수명 관리

## 인터페이스의 구현

IObserver는 3개의 메서드가 정의되어 있다.

~~~c#
public interface IObserver<T> {
    void OnNext(T value);
    void OnError(Exception error);
    void OnCompleted();
}
~~~

* OnNext: 일반적인 이벤트 발생 시 통지
* OnError: 오류 발생 시 통지
* OnCompleted: 스트림 종료 통지

IObservable에는 Subscribe 1개의 메서드가 정의되어 있다. 다음은 오버로드될 수 있는 시그니처이다.

| Subscribe 오버로드                                           | 기능                           |
|--------------------------------------------------------------|--------------------------------|
| Subscribe(IObserver observer)                                | 기본형                         |
| Subscribe()                                                  | 모든 메시지를 무시한다         |
| Subscribe(Action onNext)                                     | OnNext만 처리                  |
| Subscribe(Action onNext, Action onError)                     | OnNext & OnError               |
| Subscribe(Action onNext, Action onCompleted)                 | OnNext & OnCompleted           |
| Subscribe(Action onNext, Action onError, Action onCompleted) | OnNext & OnError & OnCompleted |

* OnNext

통상적인 이벤트가 발생했을 때 사용하는 메서드이다.

* OnError

스트림 처리 중 예외가 발생할 때 사용하는 메서드이다. 오류가 생긴다면 이 스트림은 그 시점에서 Subscribe가 종료된다.

특정 예외 발생 시 스트림을 다시 살릴 수 있는 방법이 있다. OnErrorRetry를 사용하는 것이다.

~~~c#
var stringSubject = new Subject<string>();

void Start() {
    stringSubject.Select(str => int.Parse(str))
    .OnErrorRetry((FormatException ex) => Debug.Log("예외 발생으로 다시 실행"))
    .Subscribe(x => Debug.Log($"성공: {x}"), ex => Debug.Log($"예외: {ex.ToString()}"));
}
~~~

그 외에 예외 관련 오퍼레이터는 다음과 같은 것들이 있다.

| Operators    | 기능                                                             |
|--------------|------------------------------------------------------------------|
| Retry        | OnError 발생 시 다시 Subscribe                                   |
| Catch        | OnError 발생 시 다른 스트림 대체                                 |
| CatchIgnore  | OnError 발생 시 예외 처리 후 OnError 무시하고 OnCompleted로 대체 |
| OnErrorRetry | OnError 발생 시 예외 처리 후 Subscribe                           |

* OnCompleted

OnCompleted는 스트림이 완료되었으므로 더 이상 메시지를 발행하지 않는다는 메시지를 통지하는 메서드이다.

스트림을 파기하려는 시점에 OnCompleted를 발행하여 구독을 종료시킬 수 있다.

OnCompleted가 실행된 스트림은 재사용이 불가능하며 다시 호출했을 때도 OnCompleted가 실행된다.

* Dispose

IObservable의 Subscribe 메서드의 반환형은 IDisposable이다. 이 말인 즉슨, Dispose를 실행하면 스트림을 종료할 수 있다는 것이다.

OnCompleted와의 차이점은, OnCompleted는 IObservable 객체의 모든 스트림을 종료한다는 것이지만 Dispose를 이용하면 특정 스트림만 중단할 수 있다는 것이다.

## 스트림의 생명주기

스트림은 IObservable의 부분이며 IObservable이 파기되면 스트림 또한 파기된다. 즉, 스트림을 종료시키지 않고 IObservable이 존재한다면 계속 살아있다는 것이다.

만약 스트림이 파기되기 전에 스트림이 참조하고 있던 게임오브젝트가 먼저 파기된다면 스트림은 NullReferenceException 등의 오류를 발생시킬 수 있다.

이러한 사태를 대비해서 AddTo 오퍼레이터를 이용해서 지정된 게임오브젝트가 파기되면 같이 스트림을 파기한다.

~~~c#
var subject = Subject<int>();

subject.Where(x => x > 5).Subscribe(x => {}).AddTo(gameObject);
~~~


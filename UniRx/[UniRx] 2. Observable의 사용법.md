# Observable의 사용법

## Observer(관찰자) & Observable(관찰 가능한)

Observer는 Observable이라는 개념의 객체를 이용한다.

Observable 객체를 생성한다는 것은 스트림에 흘려 보내는 값 즉, 각종 이벤트들의 관찰을 시작한다는 뜻이다.

## Observable 객체 생성 방법

1. Subject 시리즈의 사용

Subject는 간단하게 스트림을 생성해서 자유롭게 이용할 수 있다.
| Subject의 종류     | 설명                                                                                               |
|--------------------|----------------------------------------------------------------------------------------------------|
| Subject\<T>         | 가장 단순한 형태이지만 가장 자주 사용한다. OnNext로 이벤트를 전달한다.                             |
| BehaviorSubject\<T> | 마지막으로 전달한 이벤트를 캐싱하고 이후에 Subscribe될 때 그 값을 전달한다.                        |
| ReplaySubject\<T>   | 과거에 전달한 모든 이벤트를 캐싱하고 이후에 Subscribe될 때 그 값을 모두 모아 전달한다.             |
| AsyncSubject\<T>    | OnNext 사용시 즉시 전달하지 않고 내부에 캐싱한 후 OnCompleted가 실행되면 마지막 OnNext를 전달한다. |

2. ReactiveProperty 시리즈의 사용

ReactiveProperty는 int나 float와 같은 일반 변수 자료형에 Subject의 기능을 붙인 것이다. 즉, ReactiveProperty의 값을 수정하는 즉시 Subscribe에 통보하여 이벤트가 발생하도록 한다.

유니티 인스펙터에 출력시킬 수도 있는데 이를 위해서는 각 자료형대로 구현한 Inspectable ReactiveProperty를 사용해야 한다.

3. 팩토리 메서드 시리즈의 사용

팩토리 메서드를 이용하면 복잡한 스트림을 쉽게 만들 수 있다. 다만 다른 편한 방법들도 많기에 많이 사용되지는 않는다.

대표적으로는 Observable.Create, Observable.Start, Observable.Timer/TimerFrame 등이 있다.

4. UniRx.Triggers 시리즈 사용

~~~c#
using UniRx.Triggers;

void Start() {
    this.UpdateAsObservable().Subscribe(_=>Debug.Log("Update"));
}
~~~

UniRx.Triggers를 이용하면 Subscribe에 사용될 콜백을 쉽게 등록할 수 있다.

Unity에는 Update 메서드 또한 종류가 많듯이 UniRx도 이것에 대응하는 다양한 Update 메서드를 제공한다.

5. Coroutine을 변환하여 사용

~~~c#
void Start() {
    Observable.FromCoroutine<int>(observer => GameTimerCoroutine(observer, 60)).Subscribe(t => Debug.Log(t));
}

private IEnumerator GameTimerCoroutine(IObserver<int> observer, int initialCount = 100) {
    while(initialCount > 0) {
        observer.OnNext(initialCount--);
        yield return new WaitForSeconds(1);
    }

    observer.OnNext(0);
    observer.OnCompleted();
}
~~~

6. UGUI 이벤트를 변환하여 사용

이것과 ReactiveProperty를 같이 잘 사용한다면 MVP(Model-View-Presenter) 패턴을 쉽게 이식할 수 있다.

~~~c#
var button = GetComponent<Button>();
button.OnClickAsObservable().Subscribe();

var inputField = GetComponent<InputField>();
inputField.OnValueChangedAsObservable().Subscribe();
input.OnEndEditAsObservable().Subscribe();
~~~

7. 기타
* ObservableWWW

Unity의 WWW를 스트림으로 취급할 수 있도록 만든 것으로 코루틴을 사용하지 않고 비동기 다운로드 처리를 할 수 있어서 유용한 기능이다.

현재 유니티는 WWW가 Deprecate되면서 사용하지 않는 것이 좋다.

* Observable.NextFrame

다음 프레임에 메시지를 전달하는 Observable을 생성한다.

* Observable.EveryUpdate 등

Update 메서드와 마찬가지로 매프레임 호출되는 Observable을 생성한다.

UniRx.Triggers의 UpdateAsObservable과는 GameObject의 종속이 되어 있지 않다는 점에서 차이가 있는데, 그렇기 때문에 Dispose 시점을 수동으로 지정해 주어야 한다.


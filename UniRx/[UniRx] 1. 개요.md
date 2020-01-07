# UniRx

## UniRx란?

>Reactive Extensions for Unity

Functional Reactive Programming(FRP)를 C#에 사용할 수 있게 만든 .Net Reactive Extensions를 Unity 전용으로 최적화하여 오픈 소스로 공개한 라이브러리이다.

## Reactive Programming이란?

비동기적 데이터 흐름을 처리하는 프로그래밍 기법이라는 뜻으로 모든 처리를 비동기적 데이터 스트림으로 간주, 관찰자 패턴을 활용해서 비동기 이벤트를 처리한다.

## UniRx를 사용하는 이유

UniRx를 사용하면 다음과 같은 장점을 얻을 수 있다.

1. 시간 및 타이밍의 관리
2. 코드의 가독성
3. 코드 라인 수의 개선

다음 작성하는 두 코드는 더블 클릭을 판단하는 작업을 수행하는 코드이지만 코드의 가독성과 라인 수는 확연하게 드러난다.

~~~c#
public Text text;

bool isClicked = false;
float clickTime = 0.0f;
float checkTime;

void Update() {
    if(Input.GetMouseButtonDown(0)) {
        if(isClicked && Time.time - checkTime <= 0.3f) {
            text.text = "Double Clicked!";
            checkTime = 0.0f;
            isClicked = false;
        }
        else {
            isClicked = true;
            checkTime = Time.time;
        }
    }
}
~~~

~~~c#
public Text text;

void Start() {
    var clickStream = UpdateAsObservable().Where(_ => Input.GetMouseButtonDown(0));

    clickStream.Buffer(clickStream.Throttle(TimeSpan.FromMilliseconds(300)))
    .Where(x => x.Count >= 2)
    .SubscribeToText(text, x => $"Double Clicked! Click Count = {x.Count}");
}
~~~

## Rx의 핵심 작동 원리

1. 스트림을 정의한다.
2. 오퍼레이터로 스트림의 데이터를 가공한다.
3. 가공한 데이터를 통지(Subscribe)받아서 로직에 태운다.


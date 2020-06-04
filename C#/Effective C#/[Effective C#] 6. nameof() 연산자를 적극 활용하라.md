분산 시스템의 대중화로 서로 다른 시스템 사이에 데이터를 주고 받아야 하는 일이 많아졌다. 다른 플랫폼, 다른 언어라는 차이를 극복하기 위해서 이름이나 문자열 식별자에 의존하는 간단한 라이브러리들이 많이 사용된다. 다만 이 방식은 타입 정보를 손실한다는 단점이 있다.

nameof() 연산자는 심볼 그 자체를 해당 심볼을 포함하는 문자열로 대체해준다. 다음은 nameof()를 이용한 INotifyPropertyChanged 인터페이스의 구현부이다.
~~~c#
public string Name {
    get { return name; }
    set {
        if(value != name) {
            name = value;
            PropertyChanged?.Invoke(this,
                new PropertyChangedEventArgs(nameof(Name)));
        }
    }
}

private string name;
~~~

nameof() 연산자를 통해서 이름 바꾸기 작업을 수행할 때도 실수를 줄일 수 있다.
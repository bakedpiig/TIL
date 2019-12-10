생성자 내에서 코드가 중복되는 부분이 있다면 공용으로 사용할 수 있는 생성자를 작성하면서 해결할 수 있다. 다음은 다른 생성자를 호출하여 초기화 과정을 미루는 예시이다.

~~~c#
public class MyClass {
    private List<ImportantData> coll;
    private string name;

    public MyClass(): this(0,"") {}

    public MyClass(int initialCount = 0, string name = "") {
        coll = (initialCount > 0) ?
            new List<ImportantData>(initialCount) :
            new List<ImportantData>();
        
        this.name = name;
    }
}
~~~

예시와 같이 기본값을 갖는 매개변수를 취하는 생성자를 작성한다면 생성자를 여러 개 만들 필요는 없겠지만 이 타입을 사용하는 코드와의 결합도가 높아진다는 단점 또한 있다.

초기화 코드가 중복되는 것을 최소화하라고 해서 공통된 코드를 하나의 메서드로 빼는 것은 좋지 않은 판단이다. 만약 중복되는 코드를 유틸리티 메서드로 뺐을 때 다음과 같은 순서로 초기화된다.

1. 모든 인스턴스 변수에 대한 초기화 코드가 추가되어 실행된다.
2. 베이스 클래스 생성자가 호출된다.
3. 유틸리티 메서드가 호출된다.

중요한 점은 중복 코드가 제거되지 않는다는 것이다. 또한 유틸리티 메서드로드 readonly 필드가 초기화되지 않는다는 점 또한 있다.
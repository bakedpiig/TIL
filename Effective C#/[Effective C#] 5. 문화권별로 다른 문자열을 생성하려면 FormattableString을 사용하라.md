FormattableString은 문화권에 따라서 다른 문자열을 만들어 낼 수 있다.

~~~c#
var str = $"It's the {DateTime.Now.Day} of the {DateTime.Now.Month} month";
~~~

이렇게 작성했을 때 컴파일러는 컴파일타임의 몇 가지 조건을 고려해서 string, 혹은 FormattableString 객체로 생성하는 다른 코드를 생성한다. FormattableString이 되는 조건은 다음과 같다.

1. string 매개변수를 받는 메서드에 이 변수를 전달하지 말 것
2. string, FormattableString을 모두 받아들일 수 있는 오버로딩 메서드를 작성하지 말 것

FormattableString 타입의 객체를 이용하여 문화권과 언어를 지정하여 문자열을 생성하는 방법은 다음과 같다.
~~~c#
public static string ToGerman(FormattableString src) {
    return string.Format(null, System.Globalization.CultureInfo.CreateSpecificCulture("de-de"), src.Format, src.GetArguments());
}
~~~

메서드 내에서는 언어와 문화권을 지정하여 FormattableString을 최종적으로 문자열로 변경하여 반환한다.
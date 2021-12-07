import scala.collection.mutable.ListBuffer
import scala.io.Source
import scala.util.Using

def solution(xs: List[Int]): Int = {
    var prev = Int.MaxValue
    var count = 0
    for (x <- xs) {
        if (prev < x) {
            count += 1
        }
        prev = x
    }
    count
}

@main
def main = {
    var xs = ListBuffer[Int]()
    for (line <- Source.stdin.getLines) {
        xs += line.toInt
    }
    println(solution(xs.toList))
}

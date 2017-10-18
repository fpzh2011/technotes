# maven概览

maven的本地仓库是目录`~/.m2/repository`，所有的maven构件（artifact）都存储到该仓库中，以方便在多个项目间重用。

`$M2_HOME/conf/setting.xml`是全局范围的，整台机器的所有用户都会受该配置文件的影响。
`~/.m2/setting.xml`是用户范围的，只有相应用户才会受到影响。

## 配置阿里云仓库

http://www.cnblogs.com/charlesblc/p/6104804.html
修改`~/.m2/setting.xml`
```xml
<mirrors>
  <mirror>
    <id>alimaven</id>
    <name>aliyun maven</name>
    <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
    <mirrorOf>central</mirrorOf>
  </mirror>
</mirrors>
```

## skip test

```
# 不执行测试用例，也不编译测试用例类。
-Dmaven.test.skip=true

# 不执行测试用例，但编译测试用例类生成相应的class文件至target/test-classes下。
-DskipTests
```

## 指定编译个别类

```
<plugin>
  <artifactId>maven-compiler-plugin</artifactId>
    <configuration>
      <includes>
        <include>Test.java</include>
        <include>com/Service.java</include>
      </includes>
```


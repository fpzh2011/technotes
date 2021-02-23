# k8s

Kubernetes Objects
https://kubernetes.io/docs/concepts/overview/working-with-objects/kubernetes-objects/

Namespaces
https://kubernetes.io/docs/concepts/overview/working-with-objects/namespaces/

Labels and Selectors
https://kubernetes.io/docs/concepts/overview/working-with-objects/labels/

Annotations
https://kubernetes.io/docs/concepts/overview/working-with-objects/annotations/

Pod
A Pod encapsulates an application’s container (or, in some cases, multiple containers), storage resources, a unique network identity (IP address), as well as options that govern how the container(s) should run. 
同一pod中的多个容器，共享网络和存储。
Pods provide two kinds of shared resources for their constituent containers: networking and storage.
一个pod被分配一个唯一的ip地址。pod内的多个容器之间可以用localhost通讯。
Each Pod is assigned a unique IP address for each address family.
Modifying the pod template or switching to a new pod template has no effect on the Pods that already exist. Pods do not receive template updates directly; instead, a new Pod is created to match the revised pod template.
https://kubernetes.io/docs/concepts/workloads/pods/pod-overview/

StatefulSets
https://kubernetes.io/docs/concepts/workloads/controllers/statefulset/






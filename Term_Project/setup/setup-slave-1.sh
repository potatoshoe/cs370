#! /bin/bash

source "$(dirname $0)/config.sh"

fn_main () {

    if [ "$(whoami)" != "root" ]; then
        die "This script must be run as root."
    fi

    fn_install_packages
    fn_setup_user
    fn_setup_hosts
    fn_setup_nfs_client
    fn_setup_ssh_server

}

die () {
    #statements
    echo "ERROR: $1"
    exit 1
}

fn_setup_user () {

    echo "Creating user '$MPIUSER_USERNAME' with default password '$MPIUSER_PASSWORD'..."

    adduser --disabled-password --gecos "" $MPIUSER_USERNAME
    echo $MPIUSER_USERNAME:$MPIUSER_PASSWORD | chpasswd

    echo "Done."

}

fn_setup_hosts () {

    echo "Configuring hostnames..."

    hostname $SLAVE1_HOST
    echo "$SLAVE1_HOST" > /etc/hostname

    cat <<EOF > /etc/network/interfaces
source-directory /etc/network/interfaces.d

auto lo
iface lo inet loopback

auto eth0
iface eth0 inet static
    address $SLAVE1_IP
    netmask $SUBNET_MASK
    gateway $GATEWAY_IP
    nameserver $DNS_SERVERS

allow-hotplug wlan0
iface wlan0 inet manual
    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf

allow-hotplug wlan1
iface wlan1 inet manual
    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
EOF

    cat <<EOF > /etc/hosts
127.0.0.1   localhost
127.0.1.1   $SLAVE1_HOST

$SLAVE1_IP  $SLAVE1_HOST
$MASTER_IP  $MASTER_HOST
EOF

    systemctl restart networking

    echo "Done."

}

fn_install_packages () {

    echo "Performing updates and installing required packages..."

    apt-get update
    apt-get dist-upgrade -y
    apt-get install -y \
    mpich2 \
    libmpich2-dev \
    openssh-server \
    openssh-client \
    nfs-common \
    vim \
    zsh

    echo "Done."
}

fn_setup_nfs_client () {

    echo "Setting up NFS server and shared folder..."

    if [ ! -d "$SHARED_DIR_PATH" ]; then
        mkdir $SHARED_DIR_PATH
        chown $MPIUSER_USERNAME:$MPIUSER_USERNAME $SHARED_DIR_PATH
        chmod 775 $SHARED_DIR_PATH
        echo "master:$SHARED_DIR_PATH $SHARED_DIR_PATH nfs" >> /etc/fstab
    fi

    echo "Done."

}

fn_setup_ssh_server () {

    echo "Adding SSH public key..."

    systemctl enable ssh.service

    if [ ! -d "$MPIUSER_HOME/.ssh" ]; then
        mkdir $MPIUSER_HOME/.ssh
        chown $MPIUSER_USERNAME:$MPIUSER_USERNAME $MPIUSER_HOME/.ssh
        chmod 700 $MPIUSER_HOME/.ssh
    fi

    if [ ! -e "$MPIUSER_HOME/.ssh/authorized_keys" ]; then
        touch $MPIUSER_HOME/.ssh/authorized_keys
        chown $MPIUSER_USERNAME:$MPIUSER_USERNAME $MPIUSER_HOME/.ssh/authorized_keys
        chmod 644 $MPIUSER_HOME/.ssh/authorized_keys
    fi

    cat $SCRIPT_DIR/id_rsa.pub > $MPIUSER_HOME/.ssh/authorized_keys

    echo "Done."
}

fn_main

FROM ranmufei/phpenv-mssqrv-mysql-oracle:v6
MAINTAINER ranmufei <ranmufei@qq.com>

# installing required stuff
#RUN apt-get update \
#    && apt-get install -y vim unzip libaio-dev libmcrypt-dev git \
#    && apt-get clean -y

# workdir

#WORKDIR /var/www/

#ADD ./000-default.conf /etc/apache2/sites-enabled/

#RUN a2enmod rewrite
# download 
ADD  ./ /var/www/html/ 

RUN cd /var/www/html/ && cp ext_skel /usr/include/php/20151012/ext/ && chmod +xw /usr/include/php/20151012/ext/ 
RUN cp ./skeleton /usr/include/php/20151012/ext/  -R

RUN cd /usr/include/php/20151012/ext/ && chmod 777 ext_skel && ./ext_skel --extname=my_dynamic && ./ext_skel --extname=dh_nvr

RUN cd /var/www/html/ && cp ./my_dynamic/* /usr/include/php/20151012/ext/my_dynamic/ -R \
&& cp ./dh_nvr/* /usr/include/php/20151012/ext/dh_nvr/ -rvf \
&& cp ./lib/* /usr/lib/ -rvf

RUN cd /usr/include/php/20151012/ext/my_dynamic/ && /usr/bin/phpize7.0 && ./configure --with-php-config=/usr/bin/php-config7.0 \
&& make && make install

RUN cd /usr/include/php/20151012/ext/dh_nvr/ && /usr/bin/phpize7.0 && ./configure --with-php-config=/usr/bin/php-config7.0 \
&& make && make install

RUN echo '\r\nextension=my_dynamic.so' >> /etc/php/7.0/apache2/php.ini
RUN echo '\r\nextension=dh_nvr.so' >> /etc/php/7.0/apache2/php.ini
 
RUN echo '\r\nextension=my_dynamic.so' >> /etc/php/7.0/cli/php.ini
RUN echo '\r\nextension=dh_nvr.so' >> /etc/php/7.0/cli/php.ini


# workdir

WORKDIR /var/www/

ADD ./000-default.conf /etc/apache2/sites-enabled/

RUN a2enmod rewrite


# claer

RUN rm /var/www/html/* -rvf 



RUN rm /var/www/ -rvf
RUN cd /var/www/ && mkdir public && cd public && echo "hi ran!" > index.html
